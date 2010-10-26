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

#include "LoginTest.hh"
#include "server_lib.hh"

bool db_init() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("swar");
        db.setUserName("root");
        db.setPassword("");
        return db.open();
}

int main(int argc, char ** argv)
{
	CppUnit::TextUi::TestRunner runner;
	pid_t pid;

	pid = fork();
	if (pid == 0) {
		// we are in the child, connect to the db and start the server
		int argc_ = 5;
		char* argv_[] = {
			"",
			"-ORBendPoint",
			"giop:tcp::31337",
			"-ORBnativeCharCodeSet",
			"UTF-8"
		};
		db_init();
		corba_run(argc_, argv_);
	}
	// give a little time to start up
	sleep(1);

	runner.addTest( LoginTest::suite() );
	runner.run();

	// kill the server
	kill(pid, 15);

	return 0;
}
