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
}

VersionControl::UserAdmin_ptr AdminImpl::addUser(const char* /*name*/)
{
	cout << "TODO: AdminImpl::addUser()" << endl;
}

VersionControl::UserAdminSeq* AdminImpl::getUsers()
{
	cout << "TODO: VersionControl::UserAdminSeq()" << endl;
}

void AdminImpl::removeUser(VersionControl::UserAdmin_ptr /*user*/)
{
	cout << "TODO: AdminImpl::removeUser()" << endl;
}

VersionControl::ModelAdmin_ptr AdminImpl::getModelAdmin(VersionControl::Model_ptr /*target*/)
{
	cout << "TODO: AdminImpl::getModelAdmin()" << endl;
}
