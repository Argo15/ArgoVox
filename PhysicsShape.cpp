#include "PhysicsShape.h"

PhysicsShape::PhysicsShape(PhysicsShapeType type) 
{
	this->m_type=type;
}

btCollisionShape *PhysicsShape::getBoxShape()
{
	return new btBoxShape(btVector3(m_nScale[0],m_nScale[1],m_nScale[2]));
}

btCollisionShape *PhysicsShape::getSphereShape()
{
	btSphereShape *sphere = new btSphereShape(1.0);
	sphere->setLocalScaling(btVector3(m_nScale[0],m_nScale[1],m_nScale[2]));
	return sphere;
}

btCollisionShape *PhysicsShape::getCylinderShape()
{
	return new btCylinderShape(btVector3(m_nScale[0],m_nScale[1],m_nScale[2]));
}

btCollisionShape *PhysicsShape::getCollisionShape()
{
	switch (m_type)
	{
		case BOX:
			return getBoxShape();
		case CYLINDER:
			return getCylinderShape();
		case SPHERE:
			return getSphereShape();
		default:
			return 0;
	}
}

PhysicsShapeType PhysicsShape::getShapeType() 
{
	return m_type;
}