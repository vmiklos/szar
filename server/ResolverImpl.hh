#ifndef _RESOLVERIMPL_H
#define _RESOLVERIMPL_H
#include <iostream>
#include <VersionControl.hh>

using namespace std;

class ResolverImpl : public POA_VersionControl::Resolver
{
public:
	inline ResolverImpl() {}
	virtual char* merge(const char* base, const ::VersionControl::NamedModel& modela, const ::VersionControl::NamedModel& modelb);
};
#endif
