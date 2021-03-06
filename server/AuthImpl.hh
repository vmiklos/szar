#ifndef _AUTHIMPL_H
#define _AUTHIMPL_H
#include <iostream>
#include <VersionControl.hh>
#include <RootImpl.hh>
#include <QtSql>
#include <QCryptographicHash>

using namespace std;

/// Implementation of the Auth interface.
class AuthImpl : public POA_VersionControl::Auth
{
public:
	inline AuthImpl() {}
	virtual VersionControl::Root_ptr login(const char* username, const char* password);
};
#endif
