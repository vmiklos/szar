/// Tests the Model interface. Status: tests everything.
class ModelTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(ModelTest);
	CPPUNIT_TEST(testGetName);
	CPPUNIT_TEST(testGetRevisions);
	CPPUNIT_TEST(testGetRevision);
	CPPUNIT_TEST(testGetCurrentRevision);
	CPPUNIT_TEST(testCommit);
	CPPUNIT_TEST(testLock);
	CPPUNIT_TEST(testUnlock);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		test_server_init((char*)"ModelTest");

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
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		CPPUNIT_ASSERT(strcmp("model", model->getName()) == 0);
	}

	void testCommit()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);

		// base is now 1, still using 0 results in an exception where head is set to 1
		int found = 0;
		try {
			model->commit("data2", 0);
		} catch (VersionControl::NotUptodateException& e) {
			found = 1;
			CPPUNIT_ASSERT(e.head->getNumber() == 1);
		}
		CPPUNIT_ASSERT(found == 1);

		// using 1 as base works fine
		model->commit("data", 1);
	}

	void testGetCurrentRevision()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(strcmp("data", model->getCurrentRevision()->getData()) == 0);
	}

	void testGetRevision()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(strcmp("data", model->getRevision(1)->getData()) == 0);
	}

	void testGetRevisions()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->commit("data", 0);
		CPPUNIT_ASSERT(model->getRevisions()->length() == 1);
	}

	void testLock()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->lock();
	}

	void testUnlock()
	{
		VersionControl::Root_var root = authref->login("admin", "admin");
		VersionControl::Admin_var admin = root->getAdmin();
		VersionControl::Model_var model = admin->addModel("model");
		model->unlock();
	}
};
