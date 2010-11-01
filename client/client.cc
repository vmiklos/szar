#include <iostream>
#include <VersionControl.hh>

using namespace std;

int main (int argc, char **argv)
{
	try {
		// corba init
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		// auth
		CORBA::Object_var obj = orb->resolve_initial_references("AuthService");
		VersionControl::Auth_var authref = VersionControl::Auth::_narrow(obj);
		if( CORBA::is_nil(authref) )
			cerr << "The object reference is nil!" << endl;

		try {
			VersionControl::Root_var root = authref->login("admin", "admin");

			/*VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::Model_var model = root->getModel("foo");
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			ma->setName("bar");*/

			/*VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::Model_var model = root->getModel("bar");
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			ma->removeUser(root->getMyUser());*/

			/*VersionControl::Model_var model = root->getModel("foo");
			VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			cout << ma->getUsers()->length() << endl;
			VersionControl::UserAccessSeq_var uas = ma->getUsers();
			VersionControl::UserAccess ua = uas[0];
			cout << ua.grantee->getName() << endl;*/

			/*VersionControl::Model_var model = root->getModel("foo");
			VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			ma->changeUserLevel(root->getMyUser(), VersionControl::Read);*/

			/*VersionControl::Model_var model = root->getModel("foo");
			VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::UserAdminSeq_var uas = admin->getUsers();
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			// FIXME if this is not a ptr, then there will
			// be a doublefree as both the sequence and the
			// access object wants to release the User
			// object
			VersionControl::UserAccess* access = new VersionControl::UserAccess();
			access->grantee = uas[1];
			access->level = VersionControl::ReadWrite;
			ma->addUser(*access);*/

			/*VersionControl::Admin_var admin = root->getAdmin();
			VersionControl::Model_var model = root->getModel("foo");
			VersionControl::ModelAdmin_var ma = admin->getModelAdmin(model);
			ma->removeModel();*/

			/*VersionControl::UserAdmin_var u = admin->addUser("john");
			cerr << admin->getUsers()->length() << endl;
			admin->removeUser(u);
			cerr << admin->getUsers()->length() << endl;*/
		}
		catch (VersionControl::AccessDenied& e) {
			cerr << "Access denied" << endl;
		}
		catch (VersionControl::DbError& e) {
			cerr << "Database error" << endl;
		}

		// corba deinit
		orb->destroy();
	}
	catch(CORBA::TRANSIENT&) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
			<< "server." << endl;
	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name() << endl;
	}
	catch(CORBA::Exception& ex) {
		cerr << "Caught CORBA::Exception: " << ex._name() << endl;
	}
	catch(omniORB::fatalException& fe) {
		cerr << "Caught omniORB::fatalException:" << endl;
		cerr << "  file: " << fe.file() << endl;
		cerr << "  line: " << fe.line() << endl;
		cerr << "  mesg: " << fe.errmsg() << endl;
	}
	return 0;
}
