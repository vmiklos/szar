#include "ModelAdminImpl.hh"

VersionControl::UserAccessSeq* ModelAdminImpl::getUsers()
{
}

void ModelAdminImpl::setName(const char* name)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("update models set name = :name where id = :id limit 1");
	q.bindValue(":name", name);
	q.bindValue(":id", mid);
	if (q.exec()) {
		if (q.numRowsAffected() == 1)
			return;
		else
			throw VersionControl::InvalidModel();
	}
	throw VersionControl::DbError();
}

void ModelAdminImpl::removeUser(VersionControl::User_ptr toRemove)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from users where username = :name");
	q.bindValue(":name", toRemove->getName());
	if (!q.exec() || !q.next())
		throw VersionControl::InvalidUser();
	QSqlRecord r = q.record();
	int trid = r.value("id").toInt();
	q.prepare("delete from acl where user_id = :uid and model_id = :mid");
	q.bindValue(":uid", trid);
	q.bindValue(":mid", mid);
	if (q.exec()) {
		if (q.numRowsAffected() == 1)
			return;
		else
			throw VersionControl::InvalidUser();
	} else {
		cerr << "ModelAdminImpl::removeUser() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

void ModelAdminImpl::changeUserLevel(VersionControl::User_ptr toChange, VersionControl::AccessLevel access)
{
}

void ModelAdminImpl::addUser(const VersionControl::UserAccess& access)
{
}

void ModelAdminImpl::removeModel()
{
}
