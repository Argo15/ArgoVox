#include "ShadowMapManager.h"

ShadowMapManager::ShadowMapManager()
{
	m_sunShadow = new CascadedShadowMap(1024);
}

void ShadowMapManager::tick(int m_nFps)
{
	m_sunShadow->buildShadowMaps();
}

CascadedShadowMap *ShadowMapManager::getSunShadow()
{
	return m_sunShadow;
}