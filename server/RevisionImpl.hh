#ifndef _REVISIONIMPL_H
#define _REVISIONIMPL_H
#include <VersionControl.hh>
#include <QtSql>
#include "UserImpl.hh"

/// Implementation of the Revision interface.
class RevisionImpl : public POA_VersionControl::Revision
{
protected:
	unsigned int revnum;
	int mid;
public:
	void setRevnum(unsigned int value);
	void setMid(int value);
	::CORBA::ULong getNumber();
	char* getData();
	VersionControl::User_ptr getAuthor();
};
#endif
