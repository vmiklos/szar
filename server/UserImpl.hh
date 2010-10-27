#ifndef _USERIMPL_H
#define _USERIMPL_H
#include <VersionControl.hh>
#include <QtSql>

class UserImpl
{
protected:
	int uid;
public:
	void setUid(int value);
	char* getName();
	::CORBA::Boolean getAdmin();
};
#endif
