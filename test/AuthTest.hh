/// Tests the Auth interface. Status: tests everything.
class AuthTest : public TestBase
{
	TEST_SUITE(AuthTest);
	CPPUNIT_TEST(testLoginSuccess);
	CPPUNIT_TEST(testLoginFailure);
	CPPUNIT_TEST_SUITE_END();

public:
	void tearDown() 
	{
		CORBA::release(authref);
		CORBA::release(obj);
		orb->destroy();
	}

	void testLoginSuccess()
	{
		int failed = 0;
		try {
			authref->login("admin", "admin");
		} catch (VersionControl::AccessDenied& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 0);
	}

	void testLoginFailure()
	{
		int failed = 0;
		try {
			authref->login("admin", "x");
		} catch (VersionControl::AccessDenied& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 1);
	}
};
