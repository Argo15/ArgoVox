#ifndef SHADOWMAPMANAGER_H
#define SHADOWMAPMANAGER_H

#include "CascadedShadowMap.h"

class CascadedShadowMap;

class ShadowMapManager {
private:
	CascadedShadowMap *m_sunShadow;

public:
	ShadowMapManager();

	void tick(int m_nFps);
	CascadedShadowMap *getSunShadow();
};

#endif