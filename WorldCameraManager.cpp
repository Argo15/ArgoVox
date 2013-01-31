#include "WorldCameraManager.h"
#include "InputManager.h"

WorldCameraManager::WorldCameraManager()
{
	m_worldCamera = new WorldCamera();
	m_freeRoamCamera = new Camera();
	m_current = m_worldCamera;
}

void WorldCameraManager::tick(int nFps)
{
	if (InputManager::getInstance()->isKeyDownOnce('c'))
	{
		if (m_current == m_worldCamera)
		{
			m_current = m_freeRoamCamera;
		} 
		else
		{
			m_current = m_worldCamera;
		}
	}
	m_current->move(120.0/(float)nFps);
}

WorldCamera *WorldCameraManager::getPlayerCamera()
{
	return m_worldCamera;
}

Camera *WorldCameraManager::getCurrentCamera()
{
	return m_current;
}