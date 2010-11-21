#ifndef _EXCEPTIONS_H
#define _EXCEPTIONS_H_
#include <QMessageBox>

#define __EXCEPTION_HANDLER(exception, title, msg) \
} catch ( exception & e) { \
	QMessageBox::critical(0, title, msg);

#define CATCH_INVALIDMODEL \
	__EXCEPTION_HANDLER(VersionControl::InvalidModel, "Invalid model", \
		"An invalid model was referenced, please refresh and try again.")

#define CATCH_INVALIDUSER \
	__EXCEPTION_HANDLER(VersionControl::InvalidUser, "Invalid user", \
		"An invalid user was referenced, please refresh and try again.")

#define CATCH_ALREADYASSIGNED \
	__EXCEPTION_HANDLER(VersionControl::UserAlreadyAssigned, "User already assigned", \
		"This user is already assigned to this model. Please refresh and use the checkboxes.")

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
