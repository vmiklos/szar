#include "AdminImpl.hh"

void AdminImpl::setUid(int value) {
	uid = value;
}

VersionControl::Model_ptr AdminImpl::addModel(const char* name)
{
	cout << "TODO: AdminImpl::addModel()" << endl;
}

VersionControl::UserAdmin_ptr AdminImpl::addUser(const char* name)
{
	cout << "TODO: AdminImpl::addUser()" << endl;
}

VersionControl::UserAdminSeq* AdminImpl::getUsers()
{
	cout << "TODO: VersionControl::UserAdminSeq()" << endl;
}

void AdminImpl::removeUser(VersionControl::UserAdmin_ptr user)
{
	cout << "TODO: AdminImpl::removeUser()" << endl;
}

VersionControl::ModelAdmin_ptr AdminImpl::getModelAdmin(VersionControl::Model_ptr target)
{
	cout << "TODO: AdminImpl::getModelAdmin()" << endl;
}
