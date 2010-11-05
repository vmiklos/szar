#include "ResolverImpl.hh"

char* ResolverImpl::merge(const char* base, const ::VersionControl::NamedModel& modela, const ::VersionControl::NamedModel& modelb)
{
	cerr << "ResolverImpl::merge()" << endl;
	return CORBA::string_dup("res");
}
