#include "test.hh"

pid_t pid = 0;

void test_server_init() {
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
		corba_run(argc_, argv_);
	}
	// give a little time to start up
	sleep(1);

}

void test_server_destroy() {
	kill(pid, 15);
}

int main(int argc, char ** argv)
{
	CppUnit::TextUi::TestRunner runner;

	runner.addTest( LoginTest::suite() );
	runner.run();

	return 0;
}
