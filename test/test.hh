#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <wait.h>

#include <iostream>
using namespace std;

#include <VersionControl.hh>

#include <QTextStream>

#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>

#include "server_lib.hh"

#define TEST_SUITE(x) \
	public: \
	using TestBase::setUp; \
	void setUp() \
	{ \
		setUp((char*)#x); \
	} \
	CPPUNIT_TEST_SUITE(x);

/// Helper class to run testcases.
class Test
{
	pid_t pid;
	void corbaInit();
	void corbaDestroy();
	void sqlConnect();
	void sqlDisconnect();
public:
	void run();
	static void sqlInit(char *name);
};

/// Base class for testcases.
class TestBase : public CppUnit::TestFixture
{
public:
	CORBA::ORB_ptr orb;
	CORBA::Object_ptr obj;
	VersionControl::Auth_ptr authref;

	void setUp(char *name)
	{
		// setup up the db
		Test::sqlInit(name);

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
};

#include "AuthTest.hh"
#include "RootTest.hh"
#include "AdminTest.hh"
#include "ModelAdminTest.hh"
#include "ResolverTest.hh"
#include "ModelTest.hh"
#include "RevisionTest.hh"
#include "UserTest.hh"
#include "UserAdminTest.hh"
