#include "ModelAdminImpl.hh"

VersionControl::UserAccessSeq* ModelAdminImpl::getUsers()
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select user_id, rights from acl where model_id = :mid");
	q.bindValue(":mid", mid);
	if (q.exec()) {
		VersionControl::UserAccessSeq *retval = new VersionControl::UserAccessSeq();
		retval->length(q.size());
		for (int i = 0; q.next(); i++) {
			QSqlRecord r = q.record();
			VersionControl::UserAccess ua;

			UserImpl *ui = new UserImpl();
			ui->setUid(r.value("user_id").toInt());
			POA_VersionControl::User_tie<UserImpl> *ut =
				new POA_VersionControl::User_tie<UserImpl>(ui);
			ua.grantee = ut->_this();

			if (r.value("rights").toString() == "ReadWrite")
				ua.level = VersionControl::ReadWrite;
			else
				ua.level = VersionControl::Read;
			(*retval)[i] = ua;
		}
		return retval;
	} else {
		cerr << "ModelAdminImpl::getUsers() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
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
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from users where username = :name");
	q.bindValue(":name", toChange->getName());
	if (!q.exec() || !q.next())
		throw VersionControl::InvalidUser();
	int tcid = q.record().value("id").toInt();
	q.prepare("update acl set rights = :rights where user_id = :uid and model_id = :mid");
	q.bindValue(":rights", (access == VersionControl::ReadWrite ? "ReadWrite" : "Read"));
	q.bindValue(":uid", tcid);
	q.bindValue(":mid", mid);
	if (q.exec()) {
		if (q.numRowsAffected() < 1)
			throw VersionControl::InvalidModel();
		return;
	}
	throw VersionControl::DbError();
}

void ModelAdminImpl::addUser(const VersionControl::UserAccess& access)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from users where username = :name");
	q.bindValue(":name", access.grantee->getName());
	if (!q.exec() || !q.next())
		throw VersionControl::InvalidUser();
	int taid = q.record().value("id").toInt();
	q.prepare("select user_id from acl where user_id = :uid and model_id = :mid");
	q.bindValue(":uid", taid);
	q.bindValue(":mid", mid);
	if (!q.exec()) throw VersionControl::DbError();
	if (q.size() > 0 && q.next()) throw VersionControl::AlreadyExistsException();
	q.prepare("insert into acl (user_id, model_id, rights) values (:uid, :mid, :rigths)");
	q.bindValue(":uid", taid);
	q.bindValue(":mid", mid);
	q.bindValue(":rights", (access.level == VersionControl::ReadWrite ? "ReadWrite" : "Read"));
	if (!q.exec()) throw VersionControl::DbError();
}

void ModelAdminImpl::removeModel()
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("delete from models where id = :mid");
	q.bindValue(":mid", mid);
	if (!q.exec()) throw VersionControl::DbError();
}
