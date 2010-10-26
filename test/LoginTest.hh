#include <iostream>
#include <string>

using namespace std;

class LoginTest : public CppUnit::TestFixture
{
public:
	void setUp()
	{
	}

	void tearDown() 
	{
	}

	void testLogin()
	{
		int argc = 5;
		char* argv[] = {
			"",
			"-ORBInitRef",
			"AuthService=corbaloc:iiop:localhost:31337/AuthService",
			"-ORBnativeCharCodeSet",
			"UTF-8"
		};
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
		CORBA::Object_var obj = orb->resolve_initial_references("AuthService");
		VersionControl::Auth_var authref = VersionControl::Auth::_narrow(obj);
		CPPUNIT_ASSERT( !CORBA::is_nil(authref) );
		authref->login("foobar", "bazqux");
		orb->destroy();
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteOfTests = new CppUnit::TestSuite( "LoginTest" );
		suiteOfTests->addTest( new CppUnit::TestCaller<LoginTest>( 
					"testLogin", 
					&LoginTest::testLogin ) );
		return suiteOfTests;
	}
};
