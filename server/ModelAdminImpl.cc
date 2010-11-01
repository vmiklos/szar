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
