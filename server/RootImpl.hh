#ifndef _USERROOT_H
#define _USERROOT_H
#include <iostream>
#include <VersionControl.hh>
#include "UserAdminImpl.hh"
#include "ModelReader.hh"
#include "ModelWriter.hh"
#include "AdminImpl.hh"
#include "ResolverImpl.hh"

using namespace std;

class RootImpl : public POA_VersionControl::Root
{
protected:
	int uid;
public:
	static VersionControl::Model_ptr modelFromId(QSqlQuery &q, int uid);
	static bool isAdmin(int uid);
	void setUid(int value);
	virtual VersionControl::Admin_ptr getAdmin();
	virtual VersionControl::ModelSeq* getModels();
	virtual VersionControl::Model_ptr getModel(const char* name);
	virtual VersionControl::UserAdmin_ptr getMyUser();
	virtual VersionControl::Resolver_ptr getResolver();
};
#endif
