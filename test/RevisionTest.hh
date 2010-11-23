/// Tests the Revision interface. Status: tests everything.
class RevisionTest : public TestBase
{
	TEST_SUITE(RevisionTest);
	CPPUNIT_TEST(testGetNumber);
	CPPUNIT_TEST(testGetData);
	CPPUNIT_TEST(testGetAuthor);
	CPPUNIT_TEST_SUITE_END();

public:
	void testGetNumber()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(model->getCurrentRevision()->getNumber() == 1);
		model->commit("data2", 1);
		CPPUNIT_ASSERT(model->getCurrentRevision()->getNumber() == 2);
	}

	void testGetData()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(strcmp("data", model->getCurrentRevision()->getData()) == 0);
		model->commit("data2", 1);
		CPPUNIT_ASSERT(strcmp("data2", model->getCurrentRevision()->getData()) == 0);
	}

	void testGetAuthor()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(strcmp("admin", model->getCurrentRevision()->getAuthor()->getName()) == 0);
	}
};
