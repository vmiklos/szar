#include "ModelReader.hh"

void ModelReader::setMid(int value) {
	mid = value;
}

char* ModelReader::getName() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT name FROM models WHERE id = :id");
	q.bindValue(":id", mid);
	if (q.exec()) {
		if (q.next())
			return strdup(q.record().value("name").toString().toUtf8());
		else
			throw VersionControl::InvalidModel();
	}
	throw VersionControl::DbError();
}

VersionControl::RevisionSeq* ModelReader::getRevisions() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT revnum FROM revisions WHERE model_id = :id "
		"ORDER BY revnum DESC");
	q.bindValue(":id", mid);
	if (q.exec()) {
		VersionControl::RevisionSeq *retval = new VersionControl::RevisionSeq();
		retval->length(q.size());
		for (int i = 0; q.next(); i++)
			(*retval)[i] = getRevision(q.record().value("revnum").toUInt());
		return retval;
	}
	throw VersionControl::DbError();
}

VersionControl::Revision_ptr ModelReader::getRevision(::CORBA::ULong number) {
	RevisionImpl *impl = new RevisionImpl();
	impl->setRevnum(number);
	impl->setMid(mid);
	return impl->_this();
}

VersionControl::Revision_ptr ModelReader::getCurrentRevision() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT revnum FROM revisions WHERE model_id = :id "
		"ORDER BY revnum DESC LIMIT 1");
	q.bindValue(":id", mid);
	if (q.exec()) {
		if (q.next())
			return getRevision(q.record().value("revnum").toUInt());
		else
			throw VersionControl::InvalidModel();
	}
	throw VersionControl::DbError();
}

void ModelReader::commit(const char* /*data*/, ::CORBA::ULong /*base*/) {
	throw VersionControl::AccessDenied();
}

void ModelReader::lock() {
	throw VersionControl::AccessDenied();
}

void ModelReader::unlock() {
	throw VersionControl::AccessDenied();
}
