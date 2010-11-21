#include "RootImpl.hh"

void RootImpl::setUid(int value) {
	uid = value;
}

bool RootImpl::isAdmin(int uid) {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT admin FROM users WHERE id = :uid");
	q.bindValue(":uid", uid);
	if (q.exec() && q.next()) {
		return q.record().value("admin").toBool();
	} else {
		cerr << "RootImpl::isAdmin() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::Model_ptr RootImpl::modelFromId(QSqlQuery &q, int uid) {
	QSqlRecord r = q.record();
	int mid = r.value("id").toInt();
	bool writable = isAdmin(uid);
	if (!writable) {
		QString rights = r.value("rights").toString();
		cout << "Result: ID = " << mid << " rights = " << rights.toStdString() << endl;
		writable = rights == "ReadWrite";
	}
	if (writable) {
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
	throw VersionControl::DbError();
}

VersionControl::ModelSeq* RootImpl::getModels() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	try {
		if (isAdmin(uid)) {
			if (!q.exec("SELECT m.id AS id FROM models m")) throw VersionControl::DbError();
		} else {
			q.prepare("SELECT m.id AS id, a.rights AS rights FROM models m JOIN acl a "
					"ON a.model_id = m.id WHERE a.user_id = :uid ORDER BY m.name ASC");
			q.bindValue(":uid", uid);
			if (!q.exec()) throw VersionControl::DbError();
		}
	} catch(VersionControl::DbError& e) {
		cerr << "RootImpl::getModels() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
		throw;
	}
	VersionControl::ModelSeq *retval = new VersionControl::ModelSeq();
	retval->length(q.size());
	for (int i = 0; q.next(); i++) (*retval)[i] = modelFromId(q, uid);
	return retval;
}

VersionControl::Model_ptr RootImpl::getModel(const char* name) {
	cout << "[debug] getModel(name = \"" << name << "\")" << endl;
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	if (isAdmin(uid)) {
		q.prepare("SELECT m.id AS id FROM models m WHERE m.name = :name");
	} else {
		q.prepare("SELECT m.id AS id, a.rights AS rights FROM models m JOIN acl a "
			"ON a.model_id = m.id WHERE m.name = :name AND a.user_id = :uid");
		q.bindValue(":uid", uid);
	}
	q.bindValue(":name", QString::fromUtf8(name));
	if (q.exec()) {
		if (q.size() < 1) {
			cout << "Result: model not found" << endl;
			throw VersionControl::InvalidModel();
		} else {
			if (q.next()) return modelFromId(q, uid);
		}
	} else {
		cerr << "RootImpl::getModel() Error occured during SQL query: " << q.lastError().text().toStdString() << endl;
	}
	throw VersionControl::DbError();
}

VersionControl::UserAdmin_ptr RootImpl::getMyUser() {
	UserAdminImpl *uai = new UserAdminImpl();
	uai->setUid(uid);
	uai->setAdminUid(uid);
	POA_VersionControl::UserAdmin_tie<UserAdminImpl> *uat =
		new POA_VersionControl::UserAdmin_tie<UserAdminImpl>(uai);
	return uat->_this();
}

VersionControl::Resolver_ptr RootImpl::getResolver() {
	ResolverImpl *impl = new ResolverImpl();
	return impl->_this();
}
