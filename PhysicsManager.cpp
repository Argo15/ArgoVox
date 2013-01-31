#include "PhysicsManager.h"

PhysicsManager::PhysicsManager()
{
	m_worldCameras = new WorldCameraManager();
	m_worldBulletManager = new BulletManager();
}

void PhysicsManager::tick(int nFps)
{
	m_worldBulletManager->tick(nFps);
	m_worldCameras->tick(nFps);
}

WorldCameraManager *PhysicsManager::getWorldCameras() 
{
	return m_worldCameras;
}

BulletManager *PhysicsManager::getBulletManager() 
{
	return m_worldBulletManager;
}