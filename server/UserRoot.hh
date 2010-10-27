#ifndef _USERROOT_H
#define _USERROOT_H
#include <iostream>
#include <VersionControl.hh>
#include "MyUserAdmin.hh"

using namespace std;

class UserRoot : public POA_VersionControl::Root
{
protected:
	int uid;
public:
	void setUid(int value);
	virtual VersionControl::Admin_ptr getAdmin();
	virtual VersionControl::ModelSeq* getModels();
	virtual VersionControl::Model_ptr getModel(const char* name);
	virtual VersionControl::UserAdmin_ptr getMyUser();
};
#endif
