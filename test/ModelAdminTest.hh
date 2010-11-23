/// Tests the ModelAdmin interface. Status: tests everything.
class ModelAdminTest : public TestBase
{
	TEST_SUITE(ModelAdminTest);
	CPPUNIT_TEST(testGetUsers);
	CPPUNIT_TEST(testSetName);
	CPPUNIT_TEST(testAddUser);
	CPPUNIT_TEST(testRemoveUser);
	CPPUNIT_TEST(testChangeUserLevel);
	CPPUNIT_TEST(testRemoveModel);
	CPPUNIT_TEST_SUITE_END();

public:
	void testGetUsers()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 0);
	}

	void testSetName()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		ma->setName("bar");
		CPPUNIT_ASSERT(!strcmp(model->getName(), "bar") == 1);
	}

	void testAddUser()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::UserAdminSeq_var uas = admin->getUsers();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		VersionControl::UserAccess access;
		access.grantee = VersionControl::UserAdmin::_duplicate(uas[1]);
		access.level = VersionControl::ReadWrite;
		try {
			ma->addUser(access);
		} catch (VersionControl::InvalidUser& e) {
			cerr << "invalid user" << endl;
			CPPUNIT_ASSERT(0);
		} catch (VersionControl::UserAlreadyAssigned& e) {
			cerr << "already exists" << endl;
			CPPUNIT_ASSERT(0);
		} catch (VersionControl::DbError& e) {
			cerr << "dberror" << endl;
			CPPUNIT_ASSERT(0);
		}
		CPPUNIT_ASSERT(ma->getUsers()->length() == 1);
	}

	void testRemoveUser()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::UserAdminSeq_var uas = admin->getUsers();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		VersionControl::UserAccess access;
		access.grantee = VersionControl::UserAdmin::_duplicate(uas[1]);
		access.level = VersionControl::ReadWrite;
		CPPUNIT_ASSERT(ma->getUsers()->length() == 0);
		ma->addUser(access);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 1);
		ma->removeUser(uas[1]);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 0);
	}

	void testChangeUserLevel()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		VersionControl::UserAccess access;
		access.grantee = root->getMyUser();
		access.level = VersionControl::ReadWrite;
		ma->addUser(access);
		ma->changeUserLevel(root->getMyUser(), VersionControl::Read);
	}

	void testRemoveModel()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		ma->removeModel();
		int found = 0;
		try {
			ma->removeModel();
		} catch (VersionControl::InvalidModel& e) {
			found = 1;
		}
		CPPUNIT_ASSERT(found == 1);
	}
};
