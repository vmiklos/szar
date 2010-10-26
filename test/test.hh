#include <unistd.h>
#include <sys/types.h>
#include <signal.h>

#include <iostream>
using namespace std;

#include <cppunit/ui/text/TestRunner.h>

#include <VersionControl.hh>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/extensions/HelperMacros.h>

#include "server_lib.hh"

void test_server_init();
void test_server_destroy();

#include "LoginTest.hh"
