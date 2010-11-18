/// Tests the Admin interface. Status: everything tested.
class AdminTest : public TestBase
{
	CPPUNIT_TEST_SUITE(AdminTest);
	CPPUNIT_TEST(testAddModelSuccess);
	CPPUNIT_TEST(testAddModelFailure);
	CPPUNIT_TEST(testAddUserSuccess);
	CPPUNIT_TEST(testAddUserFailure);
	CPPUNIT_TEST(testGetUsers);
	CPPUNIT_TEST(testRemoveUser);
	CPPUNIT_TEST(testGetModelAdmin);
	CPPUNIT_TEST_SUITE_END();

public:
	using TestBase::setUp;

	void setUp()
	{
		setUp((char*)"AdminTest");
	}

	void tearDown() 
	{
		CORBA::release(authref);
		CORBA::release(obj);
		orb->destroy();
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

	void testAddUserSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		admin->addUser("john");
	}

	void testAddUserFailure()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		admin->addUser("john");
		int failed = 0;
		try {
			admin->addUser("john");
		} catch (VersionControl::AlreadyExistsException& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 1);
	}

	void testGetUsers()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		CPPUNIT_ASSERT(admin->getUsers()->length() == 1);
	}

	void testRemoveUser()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::UserAdmin_var u = admin->addUser("john");
		CPPUNIT_ASSERT(admin->getUsers()->length() == 2);
		admin->removeUser(u);
		CPPUNIT_ASSERT(admin->getUsers()->length() == 1);
	}

	void testGetModelAdmin()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
	}
};
