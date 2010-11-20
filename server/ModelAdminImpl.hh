#ifndef _MODELADMINIMPL_H
#define _MODELADMINIMPL_H
#include "ModelWriter.hh"

using namespace std;

/// Implementation of the ModelAdmin interface.
class ModelAdminImpl : public ModelWriter
{
public:
	virtual VersionControl::UserAccessSeq* getUsers();
	virtual void setName(const char* name);
	virtual void removeUser(VersionControl::User_ptr toRemove);
	virtual void changeUserLevel(VersionControl::User_ptr toChange, VersionControl::AccessLevel access);
	virtual void addUser(const VersionControl::UserAccess& access);
	virtual void removeModel();
};
#endif
