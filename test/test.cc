#include "test.hh"

void Test::corbaInit() {
	pid = fork();
	if (pid == 0) {
		// start the server
		int argc_ = 5;
		char* argv_[] = {
			(char*)"",
			(char*)"-ORBendPoint",
			(char*)"giop:tcp::31337",
			(char*)"-ORBnativeCharCodeSet",
			(char*)"UTF-8"
		};
		corba_run(argc_, argv_);
		exit(1);
	}
	// give a little time to start up
	sleep(1);
}

void Test::corbaDestroy() {
	kill(pid, 15);
}

void Test::sqlInit(char *name)
{
	QSqlDatabase db = QSqlDatabase::database();

	QSqlQuery q(db);
	QString dump_name(name);
	dump_name += ".sql";
	QFile file(dump_name);
	QByteArray line;
	if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		while (!file.atEnd()) {
			line = file.readLine();
			if (!q.exec(line))
				cerr << "Test::sqlInit() warning: can't execute SQL query: "
					<< q.lastError().text().toStdString() << endl;
		}
		file.close();
	}
}

void Test::sqlConnect()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("swartest");
	db.setUserName("root");
	db.setPassword("");
	db.open();
}

void Test::sqlDisconnect()
{
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
}

void Test::run()
{
	sqlConnect();
	corbaInit();
	CppUnit::TextUi::TestRunner runner;

	runner.addTest( AuthTest::suite() );
	runner.addTest( RootTest::suite() );
	runner.addTest( AdminTest::suite() );
	runner.addTest( ModelAdminTest::suite() );
	runner.addTest( ResolverTest::suite() );
	runner.addTest( ModelTest::suite() );

	runner.run();
	corbaDestroy();
	sqlDisconnect();
}

int main()
{
	Test t;
	t.run();
}
