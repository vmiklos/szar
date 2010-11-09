#include "AdminImpl.hh"

void AdminImpl::setUid(int value) {
	uid = value;
}

VersionControl::Model_ptr AdminImpl::addModel(const char* name)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	QString qsName = QString::fromUtf8(name);
	q.prepare("SELECT id, 'ReadWrite' FROM models WHERE name = :name;");
	q.bindValue(":name", qsName);
	if (q.exec()) {
		if (q.size() > 0 && q.next()) {
			VersionControl::Model_ptr model = RootImpl::modelFromId(q, uid);
			throw VersionControl::AlreadyExistsException(model);
		} else {
			q.prepare("insert into models (name) values (:name)");
			q.bindValue(":name", qsName);
			if (!q.exec()) throw VersionControl::DbError();
			q.prepare("select id from models where name = :name");
			q.bindValue(":name", qsName);
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
	QString qsName = QString::fromUtf8(name);
	q.prepare("select id from users where username = :name");
	q.bindValue(":name", qsName);
	if (q.exec()) {
		if (q.size() > 0 && q.next()) {
			throw VersionControl::AlreadyExistsException();
		} else {
			q.prepare("insert into users (username) values (:name)");
			q.bindValue(":name", qsName);
			if (!q.exec()) {
				cerr << "AdminImpl::addUser() Error occured during SQL insert: " << q.lastError().text().toStdString() << endl;
				throw VersionControl::DbError();
			}
			int addedUid = q.lastInsertId().toInt();
			UserAdminImpl *uai = new UserAdminImpl();
			uai->setUid(addedUid);
			uai->setAdminUid(uid);
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
			int currentUid = r.value("id").toInt();
			UserAdminImpl *uai = new UserAdminImpl();
			uai->setUid(currentUid);
			uai->setAdminUid(uid);
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
	q.bindValue(":name", QString::fromUtf8(user->getName()));
	if (!q.exec() || q.numRowsAffected() < 1) throw VersionControl::DbError();
}

VersionControl::ModelAdmin_ptr AdminImpl::getModelAdmin(VersionControl::Model_ptr target)
{
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("select id from models where name = :name");
	q.bindValue(":name", QString::fromUtf8(target->getName()));
	if (q.exec()) {
		if (q.size() > 0 && q.next()) {
			QSqlRecord r = q.record();
			int mid = r.value("id").toInt();
			ModelAdminImpl *mai = new ModelAdminImpl();
			mai->setMid(mid);
			mai->setUid(uid);
			POA_VersionControl::ModelAdmin_tie<ModelAdminImpl> *mat =
				new POA_VersionControl::ModelAdmin_tie<ModelAdminImpl>(mai);
			return mat->_this();
		}
	} else {
		cerr << "AdminImpl::getModelAdmin() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}
