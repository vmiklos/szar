#include "MyUserAdmin.hh"

void MyUserAdmin::setAdmin(::CORBA::Boolean /* admin */) {
	throw VersionControl::AccessDenied();
}

void MyUserAdmin::setPassword(const char* password) {
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
