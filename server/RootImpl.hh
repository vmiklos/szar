#ifndef _USERROOT_H
#define _USERROOT_H
#include <iostream>
#include <VersionControl.hh>
#include "UserAdminImpl.hh"
#include "ModelReader.hh"
#include "ModelWriter.hh"
#include "AdminImpl.hh"

using namespace std;

class RootImpl : public POA_VersionControl::Root
{
protected:
	int uid;
	VersionControl::Model_ptr modelFromId(QSqlQuery &q);
public:
	void setUid(int value);
	virtual VersionControl::Admin_ptr getAdmin();
	virtual VersionControl::ModelSeq* getModels();
	virtual VersionControl::Model_ptr getModel(const char* name);
	virtual VersionControl::UserAdmin_ptr getMyUser();
};
#endif
