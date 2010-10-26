#include <iostream>
#include <VersionControl.hh>
#include "AuthImpl.hh"
#include <QtSql>
#include "server_lib.hh"

using namespace std;

bool db_init() {
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("swar");
	db.setUserName("root");
	db.setPassword("");
	return db.open();
}

int main(int argc, char **argv)
{
	if (!db_init()) {
		cerr << "Could not connect to MySQL" << endl;
		return 1;
	}
	corba_run(argc, argv);
	return 0;
}
