#include <iostream>
#include <echo.hh>

using namespace std;

class Echo_i : public POA_Echo
{
public:
	inline Echo_i() {}
	virtual ~Echo_i() {}
	virtual char* echoString(const char* mesg);
};


char* Echo_i::echoString(const char* mesg)
{
	return CORBA::string_dup("pong");
}

static CORBA::Boolean bindObjectToName(CORBA::ORB_ptr orb, CORBA::Object_ptr objref, const char* name)
{
	CosNaming::NamingContext_var rootContext;

	try {
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");

		rootContext = CosNaming::NamingContext::_narrow(obj);
		if(CORBA::is_nil(rootContext)) {
			cerr << "Failed to narrow the root naming context." << endl;
			return 0;
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		cerr << "Caught NO_RESOURCES exception. You must configure "
			<< "the location" << endl
			<< "of the naming service." << endl;
		return 0;
	}
	catch (CORBA::ORB::InvalidName&) {
		cerr << "Service required is invalid [does not exist]." << endl;
		return 0;
	}

	try {
		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id   = (const char*) "szoftarch";
		contextName[0].kind = (const char*) "scm";

		CosNaming::NamingContext_var testContext;
		try {
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(CosNaming::NamingContext::AlreadyBound& ex) {
			CORBA::Object_var obj;
			obj = rootContext->resolve(contextName);
			testContext = CosNaming::NamingContext::_narrow(obj);
			if( CORBA::is_nil(testContext) ) {
				cerr << "Failed to narrow naming context." << endl;
				return 0;
			}
		}

		CosNaming::Name objectName;
		objectName.length(1);
		objectName[0].id   = (const char*) name;
		objectName[0].kind = (const char*) "Object";

		// It is necessary to first try bind and then rebind, as rebind
		// on it's own will throw a NotFoundexception if the Name has
		// not already been bound. [This is incorrect behaviour - it
		// should just bind].
		try {
			testContext->bind(objectName, objref);
		}
		catch(CosNaming::NamingContext::AlreadyBound& ex) {
			testContext->rebind(objectName, objref);
		}
	}
	catch(CORBA::TRANSIENT& ex) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
			<< "naming service." << endl
			<< "Make sure the naming server is running and "
			<< "configured correctly." << endl;

		return 0;
	}
	catch(CORBA::SystemException& ex) {
		cerr << "Caught a CORBA::" << ex._name()
			<< " while using the naming service." << endl;
		return 0;
	}
	return 1;
}

int main(int argc, char **argv)
{
	try {
		// CORBA init
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
		CORBA::Object_var obj = orb->resolve_initial_references("RootPOA");
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
		PortableServer::POAManager_var pman = poa->the_POAManager();
		pman->activate();

		// create Echo instance and register
		Echo_i* myecho = new Echo_i();
		poa->activate_object(myecho);
		obj = myecho->_this();
		if(!bindObjectToName(orb, obj, "Echo"))
			return 1;
		myecho->_remove_ref();

		// Enter main loop
		orb->run();
	} catch(CORBA::TRANSIENT& ex) {
		cerr << "fatal: can't connect to the name server" << endl;
	} catch(CORBA::SystemException& ex) {
		cerr << "Caught CORBA::" << ex._name() << endl;
	} catch(CORBA::Exception& ex) {
		cerr << "Caught CORBA::Exception: " << ex._name() << endl;
	} catch(omniORB::fatalException& fe) {
		cerr << "Caught omniORB::fatalException:" << endl;
		cerr << "  file: " << fe.file() << endl;
		cerr << "  line: " << fe.line() << endl;
		cerr << "  mesg: " << fe.errmsg() << endl;
	}
	return 0;
}
