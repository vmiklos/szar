#include "UserRoot.hh"

void UserRoot::setUid(int value) {
	uid = value;
}

VersionControl::Admin_ptr UserRoot::getAdmin() {
	throw VersionControl::AccessDenied();
}

VersionControl::ModelSeq* UserRoot::getModels() {
	return NULL;
}

VersionControl::Model_ptr UserRoot::getModel(const char* name) {
	cout << "[debug] getModel(name = \"" << name << "\")" << endl;
	return VersionControl::Model::_nil();
}

VersionControl::UserAdmin_ptr UserRoot::getMyUser() {
	MyUserAdmin *mua = new MyUserAdmin();
	mua->setUid(uid);
	POA_VersionControl::UserAdmin_tie<MyUserAdmin> *uat =
		new POA_VersionControl::UserAdmin_tie<MyUserAdmin>(mua);
	return uat->_this();
}
