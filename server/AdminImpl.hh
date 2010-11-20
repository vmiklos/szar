#ifndef _ADMINIMPL_H
#define _ADMINIMPL_H
#include <iostream>
#include <VersionControl.hh>
#include <RootImpl.hh>
#include <ModelAdminImpl.hh>
#include <QtSql>
#include <QCryptographicHash>

using namespace std;

/// Implementation of the Admin interface
class AdminImpl : public POA_VersionControl::Admin
{
protected:
	int uid;
public:
	void setUid(int value);
	inline AdminImpl() {}
	virtual VersionControl::Model_ptr addModel(const char* name);
	virtual VersionControl::UserAdmin_ptr addUser(const char* name);
	virtual VersionControl::UserAdminSeq* getUsers();
	virtual void removeUser(VersionControl::UserAdmin_ptr user);
	virtual VersionControl::ModelAdmin_ptr getModelAdmin(VersionControl::Model_ptr target);
};
#endif
