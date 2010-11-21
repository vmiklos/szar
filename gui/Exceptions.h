#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H_
#include <QMessageBox>

#define __EXCEPTION_HANDLER(exception, title, msg) \
} catch ( exception & e) { \
	QMessageBox::critical(0, title, msg);

#define CATCH_DBERROR \
	__EXCEPTION_HANDLER(VersionControl::DbError, "Database error", \
		"A database error occured on the server, please try again.")

#define CATCH_ACCESSDENIED(msg) \
	__EXCEPTION_HANDLER(VersionControl::AccessDenied, "Access Denied", msg)

#define CATCH_ALREADYEXISTS(subject) \
	__EXCEPTION_HANDLER(VersionControl::AlreadyExistsException, \
	QString("Duplicate ") + subject, \
	QString("A ") + subject + " with the same name already exists.")

#endif
