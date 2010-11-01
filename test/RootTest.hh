class RootTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(RootTest);
	CPPUNIT_TEST(testGetAdminSuccess);
	CPPUNIT_TEST(testGetAdminFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		test_server_init((char*)"RootTest");

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

		test_server_destroy();

		QSqlDatabase db = QSqlDatabase::database();
		db.close();
	}

	void testGetAdminSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		root->getAdmin();
	}

	void testGetAdminFailure()
	{
		VersionControl::Root_var root = authref->login("john", "foo");
		int failed = 0;
		try {
			root->getAdmin();
		} catch (VersionControl::AccessDenied& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 1);
	}
};
