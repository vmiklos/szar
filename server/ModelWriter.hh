#ifndef _MODELWRITER_H
#define _MODELWRITER_H
#include "ModelReader.hh"
#include <iostream>

using namespace std;

class ModelWriter : public ModelReader
{
protected:
	int uid;
public:
	void setUid(int value);
	virtual void commit(const char* data, ::CORBA::ULong base);
};
#endif
