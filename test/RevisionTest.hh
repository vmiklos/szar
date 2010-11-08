/// Tests the Revision interface. Status: tests everything.
class RevisionTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(RevisionTest);
	CPPUNIT_TEST(testGetNumber);
	CPPUNIT_TEST(testGetData);
	CPPUNIT_TEST(testGetAuthor);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		Test::sqlInit((char*)"RevisionTest");

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
