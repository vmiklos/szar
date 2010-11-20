#ifndef _MODELWRITER_H
#define _MODELWRITER_H
#include "ModelReader.hh"
#include <iostream>

using namespace std;

/// Read-write implementation for the Model interface.
class ModelWriter : public ModelReader
{
protected:
	int uid;
	void checkLocking(QSqlQuery &q);
public:
	void setUid(int value);
	virtual void commit(const char* data, ::CORBA::ULong base);
	virtual void lock();
	virtual void unlock();
};
#endif
