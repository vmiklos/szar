/// Tests the User interface. Status: tests everything.
class UserTest : public TestBase
{
	TEST_SUITE(UserTest);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST(testGetAdmin);
	CPPUNIT_TEST_SUITE_END();

public:
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
