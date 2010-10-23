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
			authref->login("foobar", "bazqux");
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
