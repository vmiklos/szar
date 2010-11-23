#include <iostream>
#include <QtSql>
#include "server_lib.hh"

using namespace std;

bool db_init(int argc, char **argv) {
	bool confirm = argc < 2 || strcmp(argv[1], "-yes");
	// Builtin defaults
	QString hostName("localhost");
	QString databaseName("swar");
	QString userName("root");
	QString password("");

	// Load settings
	QSettings settings("IRC Software Development", "SQL Version Control");
	if (settings.value("mysql/host").toString() != "")
		hostName = settings.value("mysql/host").toString();
	if (settings.value("mysql/database").toString() != "")
		databaseName = settings.value("mysql/database").toString();
	if (settings.value("mysql/user").toString() != "")
		userName = settings.value("mysql/user").toString();

	// Confirm, so that settings can be modified
	string ret;
	if (confirm) {
		cout << "MySQL hostname? [" << hostName.toStdString() << "] ";
		getline(cin, ret);
	}
	if (ret.length())
		hostName = ret.c_str();
	if (confirm) {
		cout << "MySQL database name? [" << databaseName.toStdString() << "] ";
		getline(cin, ret);
	}
	if (ret.length())
		databaseName = ret.c_str();
	if (confirm) {
		cout << "MySQL username? [" << userName.toStdString() << "] ";
		getline(cin, ret);
	}
	if (ret.length())
		userName = ret.c_str();
	if (confirm) {
		cout << "MySQL password? ";
		getline(cin, ret);
	}

	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName(hostName);
	db.setDatabaseName(databaseName);
	db.setUserName(userName);
	db.setPassword(ret.c_str());
	bool success = db.open();
	if (success) {
		// Successful login, save settings
		settings.setValue("mysql/host", hostName);
		settings.setValue("mysql/database", databaseName);
		settings.setValue("mysql/user", userName);
	}
	return success;
}

int main(int argc, char **argv)
{
	if (!db_init(argc, argv)) {
		cerr << "Could not connect to MySQL" << endl;
		return 1;
	}
	corba_run(argc, argv);
	return 0;
}
