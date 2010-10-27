#ifndef _MYUSERADMIN_H
#define _MYUSERADMIN_H
#include "UserImpl.hh"
#include <QCryptographicHash>

using namespace std;

class MyUserAdmin : public UserImpl
{
public:
	void setPassword(const char* password);
	void setAdmin(::CORBA::Boolean admin);
};
#endif
