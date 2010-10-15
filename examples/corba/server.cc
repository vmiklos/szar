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
		// Obtain a reference to the root context of the Name service:
		CORBA::Object_var obj;
		obj = orb->resolve_initial_references("NameService");

		// Narrow the reference returned.
		rootContext = CosNaming::NamingContext::_narrow(obj);
		if(CORBA::is_nil(rootContext)) {
			cerr << "Failed to narrow the root naming context." << endl;
			return 0;
		}
	}
	catch (CORBA::NO_RESOURCES&) {
		cerr << "Caught NO_RESOURCES exception. You must configure omniORB "
			<< "with the location" << endl
			<< "of the naming service." << endl;
		return 0;
	}
	catch (CORBA::ORB::InvalidName&) {
		// This should not happen!
		cerr << "Service required is invalid [does not exist]." << endl;
		return 0;
	}

	try {
		// Bind a context called "test" to the root context:

		CosNaming::Name contextName;
		contextName.length(1);
		contextName[0].id   = (const char*) "test";       // string copied
		contextName[0].kind = (const char*) "my_context"; // string copied
		// Note on kind: The kind field is used to indicate the type
		// of the object. This is to avoid conventions such as that used
		// by files (name.type -- e.g. test.ps = postscript etc.)

		CosNaming::NamingContext_var testContext;
		try {
			// Bind the context to root.
			testContext = rootContext->bind_new_context(contextName);
		}
		catch(CosNaming::NamingContext::AlreadyBound& ex) {
			// If the context already exists, this exception will be raised.
			// In this case, just resolve the name and assign testContext
			// to the object returned:
			CORBA::Object_var obj;
			obj = rootContext->resolve(contextName);
			testContext = CosNaming::NamingContext::_narrow(obj);
			if( CORBA::is_nil(testContext) ) {
				cerr << "Failed to narrow naming context." << endl;
				return 0;
			}
		}

		// Bind objref with name Echo to the testContext:
		CosNaming::Name objectName;
		objectName.length(1);
		objectName[0].id   = (const char*) name;   // string copied
		objectName[0].kind = (const char*) "Object"; // string copied

		try {
			testContext->bind(objectName, objref);
		}
		catch(CosNaming::NamingContext::AlreadyBound& ex) {
			testContext->rebind(objectName, objref);
		}
		// Note: Using rebind() will overwrite any Object previously bound
		//       to /test/Echo with obj.
		//       Alternatively, bind() can be used, which will raise a
		//       CosNaming::NamingContext::AlreadyBound exception if the name
		//       supplied is already bound to an object.

		// Amendment: When using OrbixNames, it is necessary to first try bind
		// and then rebind, as rebind on it's own will throw a NotFoundexception if
		// the Name has not already been bound. [This is incorrect behaviour -
		// it should just bind].
	}
	catch(CORBA::TRANSIENT& ex) {
		cerr << "Caught system exception TRANSIENT -- unable to contact the "
			<< "naming service." << endl
			<< "Make sure the naming server is running and that omniORB is "
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
