/// Tests the UserAdmin interface. Status: tests everything.
class UserAdminTest : public TestBase
{
	TEST_SUITE(UserAdminTest);
	CPPUNIT_TEST(testSetPassword);
	CPPUNIT_TEST(testSetAdminSuccess);
	CPPUNIT_TEST(testSetAdminFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	void testSetPassword()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::UserAdmin_var ua = root->getMyUser();
		ua->setPassword("secret");
	}

	void testSetAdminSuccess()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::UserAdmin_var ua = root->getMyUser();
		ua->setAdmin(false);
	}

	void testSetAdminFailure()
	{
		VersionControl::Root_var root = authref->login("john", "john");
		VersionControl::UserAdmin_var ua = root->getMyUser();
		int found = 0;
		try {
			ua->setAdmin(true);
		} catch (VersionControl::AccessDenied& e) {
			found = 1;
		}
		CPPUNIT_ASSERT( found == 1 );
	}
};
