#ifndef _USERIMPL_H
#define _USERIMPL_H
#include <iostream>
#include <VersionControl.hh>
#include <QtSql>

using namespace std;

/// Implementation of the User interface.
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
