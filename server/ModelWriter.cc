#include "ModelWriter.hh"

void ModelWriter::setUid(int value) {
	uid = value;
}

void ModelWriter::commit(const char* data, ::CORBA::ULong base) {
	QSqlDatabase db = QSqlDatabase::database();
	if (!db.transaction()) throw VersionControl::DbError();
	try {
		QSqlQuery q(db);
		q.prepare("SELECT IFNULL(MAX(revnum), 0) AS revnum "
			"FROM revisions WHERE model_id = :mid;");
		q.bindValue(":mid", mid);
		if (!q.exec() || !q.next()) throw VersionControl::DbError();
		QSqlRecord r = q.record();
		if (r.value("revnum").toUInt() != base)
			throw VersionControl::AccessDenied(); // TODO
		q.prepare("INSERT INTO revisions (model_id, user_id, content) "
			" VALUES (:mid, :uid, :content);");
		q.bindValue(":mid", mid);
		q.bindValue(":uid", uid);
		q.bindValue(":content", QString::fromUtf8(data));
		if (!q.exec()) throw VersionControl::DbError();
	}
	catch (CORBA::Exception& ex) {
		cerr << "Exception in commit(), rollback" << endl;
		db.rollback();
		throw;
	}
	db.commit();
}
