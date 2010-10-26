#include <iostream>
#include <string>

using namespace std;

class LoginTest : public CppUnit::TestFixture
{
public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp()
	{
		int argc = 5;
		char* argv[] = {
			"",
			"-ORBInitRef",
			"AuthService=corbaloc:iiop:localhost:31337/AuthService",
			"-ORBnativeCharCodeSet",
			"UTF-8"
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

	void testLoginSuccess()
	{
		int failed = 0;
		try {
			authref->login("foobar", "bazqux");
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
			authref->login("foobarr", "bazqux");
		} catch (VersionControl::AccessDenied& e) {
			failed = 1;
		} catch (VersionControl::DbError& e) {
			failed = 2;
		}
		CPPUNIT_ASSERT(failed == 1);
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "LoginTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<LoginTest>( 
					"testLoginSuccess",
					&LoginTest::testLoginSuccess ) );
		suiteOfTests->addTest( new CppUnit::TestCaller<LoginTest>( 
					"testLoginFailure",
					&LoginTest::testLoginFailure ) );
		return suiteOfTests;
	}
};
