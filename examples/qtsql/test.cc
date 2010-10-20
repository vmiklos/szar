#include <QtSql>
#include <iostream>

using namespace std;

int main(int argc, char** argv) {
	if (argc < 3) {
		cerr << "Usage: " << argv[0] << " username password" << endl;
		return 1;
	}
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("swar");
	db.setUserName("root");
	db.setPassword("");
	bool ok = db.open();
	if (ok) {
		QSqlQuery q(db);
		q.prepare(QString("SELECT id, admin FROM users "
			"WHERE username = :username AND password = SHA1(:password)"));
		q.bindValue(QString(":username"), QVariant(argv[1]));
		q.bindValue(QString(":password"), QVariant(argv[2]));
		if (q.exec()) {
			if (q.size() < 1) {
				cout << "Result: authentication failure" << endl;
			} else {
				if (q.next()) {
					QSqlRecord r = q.record();
					QString id = r.value("id").toString();
					QString admin = r.value("admin").toString();
					cout << "Result: ID = " << id.toStdString() <<
						" admin = " << admin.toStdString() << endl;
				} else {
					cerr << "Error occured during row retrieval" << endl;
				}
			}
		} else {
			cerr << "Error occured during SQL query: " <<
				q.lastError().text().toStdString() << endl;
		}
	} else {
		cerr << "Cannot open database" << endl;
	}
	db.close();
	return 0;
}
