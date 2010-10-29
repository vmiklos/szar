#ifndef _MODELREADER_H
#define _MODELREADER_H
#include <VersionControl.hh>
#include <QtSql>
#include "RevisionImpl.hh"

class ModelReader
{
protected:
	int mid;
public:
	void setMid(int value);
	char* getName();
	VersionControl::RevisionSeq* getRevisions();
	VersionControl::Revision_ptr getRevision(::CORBA::ULong number);
	VersionControl::Revision_ptr getCurrentRevision();
	virtual void commit(const char* data, ::CORBA::ULong base);
	virtual void lock();
	virtual void unlock();
};
#endif
