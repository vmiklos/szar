#include "AdminImpl.hh"

void AdminImpl::setUid(int value) {
	uid = value;
}

VersionControl::Model_ptr AdminImpl::addModel(const char* name)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from models where name = :name");
	q.bindValue(":name", name);
	if (q.exec()) {
		if (q.size() > 0 && q.next()) {
			VersionControl::Model_ptr model = RootImpl::modelFromId(q, uid);
			throw VersionControl::AlreadyExistsException(model);
		} else {
			q.prepare("insert into models (name) values (:name)");
			q.bindValue(":name", name);
			if (!q.exec()) throw VersionControl::DbError();
			q.prepare("select id from models where name = :name");
			q.bindValue(":name", name);
			if (q.exec() && q.next())
				return RootImpl::modelFromId(q, uid);
		}
	} else {
		cerr << "AdminImpl::addModel() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::UserAdmin_ptr AdminImpl::addUser(const char* name)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from users where username = :name");
	q.bindValue(":name", name);
	if (q.exec()) {
		if (q.size() > 0 && q.next()) {
			throw VersionControl::DbError();
		} else {
			q.prepare("insert into users (username, password) values (:name, :pass)");
			q.bindValue(":name", name);
			// Just like in /etc/passwd, use UserAdmin::setPassword() later.
			q.bindValue(":pass", "x");
			if (!q.exec()) {
				cerr << "AdminImpl::addUser() Error occured during SQL insert: " << q.lastError().text().toStdString() << endl;
				throw VersionControl::DbError();
			}
			int uid = q.lastInsertId().toInt();
			UserAdminImpl *uai = new UserAdminImpl();
			uai->setUid(uid);
			POA_VersionControl::UserAdmin_tie<UserAdminImpl> *uat =
				new POA_VersionControl::UserAdmin_tie<UserAdminImpl>(uai);
			return uat->_this();
		}
	} else {
		cerr << "AdminImpl::addUser() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::UserAdminSeq* AdminImpl::getUsers()
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	if (q.exec("select id from users")) {
		VersionControl::UserAdminSeq *retval = new VersionControl::UserAdminSeq();
		retval->length(q.size());
		for (int i = 0; q.next(); i++) {
			QSqlRecord r = q.record();
			int uid = r.value("id").toInt();
			UserAdminImpl *uai = new UserAdminImpl();
			uai->setUid(uid);
			POA_VersionControl::UserAdmin_tie<UserAdminImpl> *uat =
				new POA_VersionControl::UserAdmin_tie<UserAdminImpl>(uai);
			(*retval)[i] = uat->_this();
		}
		return retval;
	} else {
		cerr << "AdminImpl::getUsers() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

void AdminImpl::removeUser(VersionControl::UserAdmin_ptr user)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("delete from users where username = :name limit 1");
	q.bindValue(":name", user->getName());
	if (!q.exec() || q.numRowsAffected() < 1) throw VersionControl::DbError();
}

VersionControl::ModelAdmin_ptr AdminImpl::getModelAdmin(VersionControl::Model_ptr /*target*/)
{
	cout << "TODO: AdminImpl::getModelAdmin()" << endl;
}
