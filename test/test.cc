#include "test.hh"

pid_t pid = 0;

bool test_db_init() {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setDatabaseName("swar");
        db.setUserName("root");
        db.setPassword("");
        return db.open();
}

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

void test_server_destory() {
	kill(pid, 15);
}

int main(int argc, char ** argv)
{
	CppUnit::TextUi::TestRunner runner;

	test_db_init();
	test_server_init();

	runner.addTest( LoginTest::suite() );
	runner.run();

	test_server_destory();

	return 0;
}
