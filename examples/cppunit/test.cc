#include <cppunit/ui/text/TestRunner.h>

#include <VersionControl.hh>
#include <cppunit/TestFixture.h>
#include <cppunit/TestAssert.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "HelloTest.hh"

int main( int argc, char **argv)
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest( HelloTest::suite() );

	runner.run();
	return 0;
}
