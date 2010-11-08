/// Tests the UserAdmin interface. Status: tests everything.
class UserAdminTest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UserAdminTest);
	CPPUNIT_TEST(testSetPassword);
	CPPUNIT_TEST(testSetAdminSuccess);
	CPPUNIT_TEST(testSetAdminFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		// setup up the db & corba server
		Test::sqlInit((char*)"UserAdminTest");

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
