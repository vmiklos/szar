/// Tests the User interface. Status: tests everything.
class UserTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UserTest);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST(testGetAdmin);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db
		Test::sqlInit((char*)"UserTest");

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

	void testGetName()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		CPPUNIT_ASSERT(strcmp("admin", root->getMyUser()->getName()) == 0);
	}

	void testGetAdmin()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		CPPUNIT_ASSERT(root->getMyUser()->getAdmin() == true);
	}
};
