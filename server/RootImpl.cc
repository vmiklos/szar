#include "RootImpl.hh"

void RootImpl::setUid(int value) {
	uid = value;
}

VersionControl::Model_ptr RootImpl::modelFromId(QSqlQuery &q) {
	QSqlRecord r = q.record();
	int mid = r.value("id").toInt();
	QString rights = r.value("rights").toString();
	cout << "Result: ID = " << mid << " rights = " << rights.toStdString() << endl;
	if (rights == "ReadWrite") {
		ModelWriter *mw = new ModelWriter();
		mw->setMid(mid);
		mw->setUid(uid);
		POA_VersionControl::Model_tie<ModelWriter> *mt =
			new POA_VersionControl::Model_tie<ModelWriter>(mw);
		return mt->_this();
	} else {
		ModelReader *mr = new ModelReader();
		mr->setMid(mid);
		POA_VersionControl::Model_tie<ModelReader> *mt =
			new POA_VersionControl::Model_tie<ModelReader>(mr);
		return mt->_this();
	}
}

VersionControl::Admin_ptr RootImpl::getAdmin() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT admin FROM users WHERE id = :uid");
	q.bindValue(":uid", uid);
	if (q.exec() && q.next()) {
		QSqlRecord r = q.record();
		int admin = r.value("admin").toInt();
		cerr << "[debug] RootImpl::getAdmin(), admin is " << admin << endl;
		if (admin) {
			AdminImpl *impl = new AdminImpl();
			impl->setUid(uid);
			return impl->_this();
		} else {
			throw VersionControl::AccessDenied();
		}
	} else {
		cerr << "RootImpl::getAdmin() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
}

VersionControl::ModelSeq* RootImpl::getModels() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT m.id AS id, a.rights AS rights FROM models m JOIN acl a "
		"ON a.model_id = m.id WHERE a.user_id = :uid ORDER BY m.name ASC");
	q.bindValue(":uid", uid);
	if (q.exec()) {
		VersionControl::ModelSeq *retval = new VersionControl::ModelSeq();
		retval->length(q.size());
		for (int i = 0; q.next(); i++) (*retval)[i] = modelFromId(q);
		return retval;
	} else {
		cerr << "RootImpl::getModels() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::Model_ptr RootImpl::getModel(const char* name) {
	cout << "[debug] getModel(name = \"" << name << "\")" << endl;
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT m.id AS id, a.rights AS rights FROM models m JOIN acl a "
		"ON a.model_id = m.id WHERE m.name = :name AND a.user_id = :uid");
	q.bindValue(":name", QString::fromUtf8(name));
	q.bindValue(":uid", uid);
	if (q.exec()) {
		if (q.size() < 1) {
			cout << "Result: model not found" << endl;
			throw VersionControl::InvalidModel();
		} else {
			if (q.next()) return modelFromId(q);
		}
	} else {
		cerr << "RootImpl::getModel() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::UserAdmin_ptr RootImpl::getMyUser() {
	MyUserAdmin *mua = new MyUserAdmin();
	mua->setUid(uid);
	POA_VersionControl::UserAdmin_tie<MyUserAdmin> *uat =
		new POA_VersionControl::UserAdmin_tie<MyUserAdmin>(mua);
	return uat->_this();
}
