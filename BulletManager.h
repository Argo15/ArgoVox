#ifndef BULLETMANAGER_H
#define BULLETMANAGER_H

#include "btBulletDynamicsCommon.h"
#include "PhysicsShape.h"

class BulletManager 
{
private:
	btDiscreteDynamicsWorld* m_dynamicsWorld;
	btRigidBody *m_playerBody;

public:
	BulletManager();

	void tick(int nFps);
	void addPhysicsShape(PhysicsShape *shape);
	void clearDynamicsWorld();
	void createPlayerBody();
	void updateDynamicsWorld(float nSpeed);
};

#endif