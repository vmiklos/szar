#include <iostream>
#include <echo.hh>

using namespace std;

static CORBA::Object_ptr getObjectReference(CORBA::ORB_ptr orb, const char *id)
{
	CosNaming::NamingContext_var rootContext;

	try {
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");

		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if( CORBA::is_nil(rootContext) ) {
			cerr << "Failed to narrow the root naming context." << endl;
			return CORBA::Object::_nil();
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
			<< "with the location" << endl
			<< "of the naming service." << endl;
		return 0;
	}
	catch(CORBA::ORB::InvalidName& ex) {
		// This should not happen!
		cerr << "Service required is invalid [does not exist]." << endl;
		return CORBA::Object::_nil();
	}

	// Create a name object, containing the name test/context:
	CosNaming::Name name;
	name.length(2);

	name[0].id   = (const char*) "test";       // string copied
	name[0].kind = (const char*) "my_context"; // string copied
	name[1].id   = (const char*) id;
	name[1].kind = (const char*) "Object";
	// Note on kind: The kind field is used to indicate the type
	// of the object. This is to avoid conventions such as that used
	// by files (name.type -- e.g. test.ps = postscript etc.)

	try {
		// Resolve the name to an object reference.
		return rootContext->resolve(name);
	}
	catch(CosNaming::NamingContext::NotFound& ex) {
		// This exception is thrown if any of the components of the
		// path [contexts or the object] aren't found:
		cerr << "Context not found." << endl;
	}
	catch(CORBA::TRANSIENT& ex) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
			<< "naming service." << endl
			<< "Make sure the naming server is running and that omniORB is "
			<< "configured correctly." << endl;

	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name()
			<< " while using the naming service." << endl;
		return 0;
	}

	return CORBA::Object::_nil();
}

int main (int argc, char **argv) 
{
	try {
		// corba init
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

		// get echo
		CORBA::Object_var obj = getObjectReference(orb, "Echo");
		Echo_var echoref = Echo::_narrow(obj);
		if( CORBA::is_nil(echoref) )
			cerr << "The object reference is nil!" << endl;

		// use init
		for (CORBA::ULong count=0; count < 3; count++) {
			CORBA::String_var src = (const char*) "ping";
			cout << "< " << (char*)src << endl;

			CORBA::String_var dest = echoref->echoString(src);

			cout << "> " << (char*)dest << endl;
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

