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
	return NULL;
}

VersionControl::Revision_ptr ModelReader::getRevision(::CORBA::ULong /*number*/) {
	return VersionControl::Revision::_nil();
}

VersionControl::Revision_ptr ModelReader::getCurrentRevision() {
	return VersionControl::Revision::_nil();
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
