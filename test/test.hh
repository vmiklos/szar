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

class Test
{
	pid_t pid;
	void corbaInit();
	void corbaDestroy();
public:
	void run();
	static void sqlInit(char *name);
};

#include "AuthTest.hh"
#include "RootTest.hh"
#include "AdminTest.hh"
#include "ModelAdminTest.hh"
#include "ResolverTest.hh"
#include "ModelTest.hh"
