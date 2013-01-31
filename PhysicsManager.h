#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "WorldCameraManager.h"
#include "BulletManager.h"

class PhysicsManager 
{
private:
	WorldCameraManager *m_worldCameras;
	BulletManager *m_worldBulletManager;

public:
	PhysicsManager();

	void tick(int nFps);
	WorldCameraManager *getWorldCameras();
	BulletManager *getBulletManager();
};

#endif