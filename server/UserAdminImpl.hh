#ifndef _USERADMINIMPL_H
#define _USERADMINIMPL_H
#include "UserImpl.hh"
#include <QCryptographicHash>

using namespace std;

class UserAdminImpl : public UserImpl
{
	int adminuid;
public:
	void setPassword(const char* password);
	void setAdmin(::CORBA::Boolean admin);
	void setAdminUid(int value);
};
#endif
