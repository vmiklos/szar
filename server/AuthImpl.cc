#include "AuthImpl.hh"

VersionControl::Root_ptr AuthImpl::login(const char* username, const char* password)
{
	QSqlDatabase db = QSqlDatabase::database();
	cerr << "[debug] LOGIN " << username << " / " << password << endl;
	QSqlQuery q(db);
	q.prepare(QString("SELECT id, admin FROM users "
		"WHERE username = :username AND password = SHA1(:password)"));
	q.bindValue(QString(":username"), QVariant(username));
	q.bindValue(QString(":password"), QVariant(password));
	if (q.exec()) {
		if (q.size() < 1) {
			cout << "Result: authentication failure" << endl;
			throw VersionControl::AccessDenied();
		} else {
			if (q.next()) {
				QSqlRecord r = q.record();
				QString id = r.value("id").toString();
				QString admin = r.value("admin").toString();
				cout << "Result: ID = " << id.toStdString() <<
					" admin = " << admin.toStdString() << endl;
				int uid = r.value("id").toInt();
				UserRoot *impl = new UserRoot();
				impl->setUid(uid);
				return impl->_this();
			}
		}
	}
	throw VersionControl::DbError();
}
