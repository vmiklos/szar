#include "UserImpl.hh"

void UserImpl::setUid(int value) {
	uid = value;
}

::CORBA::Boolean UserImpl::getAdmin() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare(QString("SELECT admin FROM users WHERE id = :id"));
	q.bindValue(QString(":id"), QVariant(uid));
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
	q.prepare(QString("SELECT username FROM users WHERE id = :id"));
	q.bindValue(QString(":id"), QVariant(uid));
	if (q.exec()) {
		if (q.next())
			return strdup(q.record().value("username").toString().toUtf8());
		else
			throw VersionControl::InvalidUser();
	}
	throw VersionControl::DbError();
}
