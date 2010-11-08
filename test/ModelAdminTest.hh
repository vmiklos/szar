/// Tests the ModelAdmin interface. Status: tests everything.
class ModelAdminTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ModelAdminTest);
	CPPUNIT_TEST(testGetUsers);
	CPPUNIT_TEST(testSetName);
	CPPUNIT_TEST(testAddUser);
	CPPUNIT_TEST(testRemoveUser);
	CPPUNIT_TEST(testChangeUserLevel);
	CPPUNIT_TEST(testRemoveModel);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db
		Test::sqlInit((char*)"ModelAdminTest");

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

	void testGetUsers()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 1);
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
		} catch (VersionControl::AlreadyExistsException& e) {
			cerr << "already exists" << endl;
			CPPUNIT_ASSERT(0);
		} catch (VersionControl::DbError& e) {
			cerr << "dberror" << endl;
			CPPUNIT_ASSERT(0);
		}
		CPPUNIT_ASSERT(ma->getUsers()->length() == 2);
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
		CPPUNIT_ASSERT(ma->getUsers()->length() == 1);
		ma->addUser(access);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 2);
		ma->removeUser(uas[1]);
		CPPUNIT_ASSERT(ma->getUsers()->length() == 1);
	}

	void testChangeUserLevel()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		ma->changeUserLevel(root->getMyUser(), VersionControl::Read);
	}

	void testRemoveModel()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("foo");
		VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
		root->getModel("foo");
		ma->removeModel();
		int found = 0;
		try {
			root->getModel("foo");
		} catch (VersionControl::InvalidModel& e) {
			found = 1;
		}
		CPPUNIT_ASSERT(found == 1);
	}
};
