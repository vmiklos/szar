#include "RevisionImpl.hh"

void RevisionImpl::setRevnum(unsigned int value) {
	revnum = value;
}

void RevisionImpl::setMid(int value) {
	mid = value;
}

::CORBA::ULong RevisionImpl::getNumber() {
	return revnum;
}

char* RevisionImpl::getData() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT content FROM revisions "
		"WHERE revnum = :revnum AND model_id = :mid");
	q.bindValue(":revnum", revnum);
	q.bindValue(":mid", mid);
	if (q.exec() && q.next())
		return strdup(q.record().value("content").toByteArray().constData());
	throw VersionControl::DbError();
}

VersionControl::User_ptr RevisionImpl::getAuthor() {
	QSqlDatabase db = QSqlDatabase::database();
	QSqlQuery q(db);
	q.prepare("SELECT user_id FROM revisions "
		"WHERE revnum = :revnum AND model_id = :mid");
	q.bindValue(":revnum", revnum);
	q.bindValue(":mid", mid);
	if (q.exec() && q.next()) {
		UserImpl *ui = new UserImpl();
		ui->setUid(q.record().value("user_id").toInt());
		POA_VersionControl::User_tie<UserImpl> *ut =
			new POA_VersionControl::User_tie<UserImpl>(ui);
		return ut->_this();
	}
	throw VersionControl::DbError();
}
