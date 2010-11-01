class AdminTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(AdminTest);
	CPPUNIT_TEST(testAddModelSuccess);
	CPPUNIT_TEST(testAddModelFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		test_server_init((char*)"AdminTest");

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

	void testAddModelSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		admin->addModel("foo");
	}

	void testAddModelFailure()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		admin->addModel("foo");
		int failed = 0;
		try {
			admin->addModel("foo");
		} catch (VersionControl::AlreadyExistsException& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 1);
	}
};
