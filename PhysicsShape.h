#ifndef PHYSICSSHAPE_H
#define PHYSICSSHAPE_H

#include "Actor.h"
#include "btBulletDynamicsCommon.h"

enum PhysicsShapeType {BOX, CYLINDER, SPHERE};

class PhysicsShape : public Actor 
{
private:
	PhysicsShapeType m_type;

	btCollisionShape *getBoxShape();
	btCollisionShape *getSphereShape();
	btCollisionShape *getCylinderShape();

public:
	PhysicsShape(PhysicsShapeType type);

	btCollisionShape *getCollisionShape();
	PhysicsShapeType getShapeType();

	glm::mat4 matrix;
};

#endif