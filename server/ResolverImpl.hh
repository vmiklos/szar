#ifndef _RESOLVERIMPL_H
#define _RESOLVERIMPL_H
#include <iostream>
#include <stdlib.h>
#include <xdiff.h>
#include <VersionControl.hh>

using namespace std;

/// Implementation of the Resolver interface.
class ResolverImpl : public POA_VersionControl::Resolver
{
public:
	inline ResolverImpl() {}
	virtual char* merge(const char* base, const VersionControl::NamedModel& modela, const VersionControl::NamedModel& modelb);
private:
	void readString(mmfile_t *ptr, const char *string);
};
#endif
