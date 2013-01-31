#ifndef ENTITY_H
#define ENTITY_H

#include <string>
using namespace std;

class Entity
{
protected:
	string *m_sName;

public:
	Entity(string *sName = 0);

	string *getName();
	void setName(string sName);
};

#endif