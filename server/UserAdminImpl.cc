#include "UserAdminImpl.hh"

void UserAdminImpl::setAdmin(::CORBA::Boolean admin) {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT admin FROM users WHERE id = :id");
	q.bindValue(":id", adminuid);
	if (q.exec() && q.next()) {
		if (q.record().value("admin").toBool()) {
			q.prepare("UPDATE users SET admin = :admin WHERE id = :id");
			q.bindValue(":id", uid);
			q.bindValue(":admin", admin);
			if (q.exec())
				return;
		} else {
			throw VersionControl::AccessDenied();
		}
	} else {
		cerr << "UserAdminImpl::setAdmin() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

void UserAdminImpl::setPassword(const char* password) {
	QSqlDatabase db = QSqlDatabase::database();
	QCryptographicHash sha1(QCryptographicHash::Sha1);
	sha1.reset();
	sha1.addData(password, strlen(password));
	QByteArray sha1pw = sha1.result().toHex();
	QSqlQuery q(db);
	q.prepare("UPDATE users SET password = :password WHERE id = :id");
	q.bindValue(":id", uid);
	q.bindValue(":password", sha1pw);
	if (!q.exec())
		throw VersionControl::DbError();
}

void UserAdminImpl::setAdminUid(int value)
{
	adminuid = value;
}
