/// Tests the Resolver interface. Status: tests everything.
class ResolverTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ResolverTest);
	CPPUNIT_TEST(testMergeSuccess);
	CPPUNIT_TEST(testMergeFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		sql_init((char*)"ResolverTest");

		// set up the client
		int argc = 5;
		char* argv[] = {
			(char*)"",
			(char*)"-ORBInitRef",
			(char*)"AuthService=corbaloc:iiop:localhost:31337/AuthService",
			(char*)"-ORBnativeCharCodeSet",
			(char*)"UTF-8"
		};
		orb = CORBA::ORB_init(argc, argv);
		obj = orb->resolve_initial_references("AuthService");
		authref = VersionControl::Auth::_narrow(obj);
		CPPUNIT_ASSERT( !CORBA::is_nil(authref) );
	}

	void tearDown() 
	{
		CORBA::release(authref);
		CORBA::release(obj);
		orb->destroy();
	}

	void testMergeSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Resolver_var resolver = root->getResolver();
		VersionControl::NamedModel modela;
		modela.name = "foo";
		modela.data = "A\nb\nc\nd\ne\nf\ng\nh\n";
		VersionControl::NamedModel modelb;
		modelb.name = "bar";
		modelb.data = "a\nb\nc\nd\ne\nf\ng\nH\n";
		char* ret = resolver->merge("a\nb\nc\nd\ne\nf\ng\nh\n", modela, modelb);
		CPPUNIT_ASSERT(strcmp(ret, "A\nb\nc\nd\ne\nf\ng\nH\n") == 0);
	}

	void testMergeFailure()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Resolver_var resolver = root->getResolver();
		VersionControl::NamedModel modela;
		modela.name = "foo";
		modela.data = "AA\nb\nc\nd\ne\nf\ng\nh\n";
		VersionControl::NamedModel modelb;
		modelb.name = "bar";
		modelb.data = "BB\nb\nc\nd\ne\nf\ng\nh\n";
		int found = 0;
		try {
			resolver->merge("a\nb\nc\nd\ne\nf\ng\nh\n", modela, modelb);
		} catch (VersionControl::ConflictException& e) {
			found = 1;
			CPPUNIT_ASSERT(strcmp(e.toResolve, "<<<<<<< foo\nAA\n=======\nBB\n>>>>>>> bar\nb\nc\nd\ne\nf\ng\nh\n") == 0);
		}
		CPPUNIT_ASSERT(found == 1);
	}
};
