#include "server_lib.hh"

void corba_run(int argc, char** argv) {
	try {
		// CORBA init
		CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);
		CORBA::Object_var obj = orb->resolve_initial_references("omniINSPOA");
		PortableServer::POA_var poa = PortableServer::POA::_narrow(obj);
		PortableServer::POAManager_var pman = poa->the_POAManager();
		pman->activate();

		// create Auth instance and activate with id
		AuthImpl* auth = new AuthImpl();
		PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId("AuthService");
		poa->activate_object_with_id(id, auth);
		cout << "Activated" << endl;

		// activate RootPOA too
		obj = orb->resolve_initial_references("RootPOA");
		poa = PortableServer::POA::_narrow(obj);
		pman = poa->the_POAManager();
		pman->activate();

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
	QSqlDatabase db = QSqlDatabase::database();
	db.close();
}
