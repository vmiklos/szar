#include "AuthImpl.hh"

VersionControl::Root_ptr AuthImpl::login(const char* username, const char* password)
{
	QSqlDatabase db = QSqlDatabase::database();
	cerr << "[debug] LOGIN " << username << " / " << password << endl;
	QCryptographicHash sha1(QCryptographicHash::Sha1);
	sha1.reset();
	sha1.addData(password, strlen(password));
	QByteArray sha1pw = sha1.result().toHex();
	QSqlQuery q(db);
	q.prepare("SELECT id, admin FROM users "
		"WHERE username = :username AND password = :password");
	q.bindValue(":username", username);
	q.bindValue(":password", sha1pw);
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
				RootImpl *impl = new RootImpl();
				impl->setUid(uid);
				return impl->_this();
			}
		}
	} else {
		cerr << "AuthImpl::login() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}
