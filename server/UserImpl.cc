#include "UserImpl.hh"

void UserImpl::setUid(int value) {
	uid = value;
}

::CORBA::Boolean UserImpl::getAdmin() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT admin FROM users WHERE id = :id");
	q.bindValue(":id", uid);
	if (q.exec()) {
		if (q.next())
			return q.record().value("admin").toBool();
		else
			throw VersionControl::InvalidUser();
	}
	throw VersionControl::DbError();
}

char* UserImpl::getName() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT username FROM users WHERE id = :id");
	q.bindValue(":id", uid);
	if (q.exec()) {
		if (q.next())
			return strdup(q.record().value("username").toString().toUtf8());
		else
			throw VersionControl::InvalidUser();
	}
	throw VersionControl::DbError();
}
