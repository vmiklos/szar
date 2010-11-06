#include "ResolverImpl.hh"

void ResolverImpl::readString(mmfile_t *ptr, const char *string)
{
	ptr->size = strlen(string);
	ptr->ptr = (char*)string;
}

char* ResolverImpl::merge(const char* base, const VersionControl::NamedModel& modela, const VersionControl::NamedModel& modelb)
{
	mmfile_t mmfs[3];
	mmbuffer_t result = {NULL, 0};
	xpparam_t xpp = {XDF_NEED_MINIMAL};
	int code;

	readString(mmfs + 0, base);
	readString(mmfs + 1, (const char*)modela.data);
	readString(mmfs + 2, (const char*)modelb.data);


	code = xdl_merge(mmfs + 0, mmfs + 1, modela.name, mmfs + 2, modelb.name,
		&xpp, XDL_MERGE_ZEALOUS, &result);

	if (code >= 0) {
		char *ret = CORBA::string_dup(result.ptr);
		free(result.ptr);
		// this is quite important, xdiff does not return a
		// null-terminated string for obvious reasons
		ret[result.size] = '\0';
		if (code == 0) {
			return ret;
		}
		throw VersionControl::ConflictException(ret);
	}
	cerr << "ResolverImpl::merge internal xdiff error, should not happen" << endl;
	throw VersionControl::DbError();
}
