#ifndef WORLDCAMERAMANAGER_H
#define WORLDCAMERAMANAGER_H

#include "WorldCamera.h"

class WorldCameraManager
{
private:
	WorldCamera *m_worldCamera;
	Camera *m_freeRoamCamera;
	Camera *m_current;

public:
	WorldCameraManager();

	void tick(int nFps);
	WorldCamera *getPlayerCamera();
	Camera *getCurrentCamera();
};

#endif