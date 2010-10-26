#include "test.hh"

pid_t pid = 0;

void test_server_init(char *name) {
	pid = fork();
	if (pid == 0) {
		QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
		db.setHostName("localhost");
		db.setDatabaseName("swartest");
		db.setUserName("root");
		db.setPassword("");
		CPPUNIT_ASSERT(db.open());

		QSqlQuery q(db);
		QString dump_name(name);
		dump_name += ".sql";
		QFile file(dump_name);
		QByteArray line;
		if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
			while (!file.atEnd()) {
				line = file.readLine();
				if (!q.exec(line))
					cerr << "test_server_init() warning: can't execute SQL query: "
						<< q.lastError().text().toStdString() << endl;
			}
			file.close();
		}

		// start the server
		int argc_ = 5;
		char* argv_[] = {
			"",
			"-ORBendPoint",
			"giop:tcp::31337",
			"-ORBnativeCharCodeSet",
			"UTF-8"
		};
		corba_run(argc_, argv_);
		exit(1);
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
