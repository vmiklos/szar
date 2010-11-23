/// Tests the Root interface. Status: everything tested.
class RootTest : public TestBase
{
	TEST_SUITE(RootTest);
	CPPUNIT_TEST(testGetAdminSuccess);
	CPPUNIT_TEST(testGetAdminFailure);
	CPPUNIT_TEST(testGetModels);
	CPPUNIT_TEST(testGetModelSuccess);
	CPPUNIT_TEST(testGetModelFailure);
	CPPUNIT_TEST(testGetMyUser);
	CPPUNIT_TEST(testGetResolver);
	CPPUNIT_TEST_SUITE_END();

public:
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

	void testGetModels()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		CPPUNIT_ASSERT(root->getModels()->length() == 1);
	}

	void testGetModelSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		root->getModel("foo");
	}

	void testGetModelFailure()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		int failed = 0;
		try {
			root->getModel("bar");
		} catch (VersionControl::InvalidModel& e) {
			failed = 1;
		}
		CPPUNIT_ASSERT(failed == 1);
	}

	void testGetMyUser()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		root->getMyUser();
	}

	void testGetResolver()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		root->getResolver();
	}
};
