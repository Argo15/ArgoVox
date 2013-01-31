#include "Entity.h"

Entity::Entity(string *sName) 
{
	this->m_sName = sName;
}

string *Entity::getName() 
{
	return m_sName;
}

void Entity::setName(string sName) 
{
	m_sName = new string(sName);
}