#ifndef CASCADEDSHADOWMAP_H
#define CASCADEDSHADOWMAP_H

#include "DepthBuffer.h"
#include "Frustum.h"
#include "Camera.h"
#include "View.h"
#include "DirectLight.h"
#include "WorldState.h"

class CascadedShadowMap
{
private:
	DepthBuffer *m_shadowMaps[4];
	glm::mat4 m_m4LightMatrix[4];
	float m_nSlices[3];
	int m_nSize;

	Camera *createLightCamera(float nSlice1, float nSlice2, Camera *camera, View *view, DirectLight *dLight);
	View *createLightView(float nSlice1, float nSlice2, Camera *camera, Camera *lightCamera, View *view, Frustum *frustum);

public:
	CascadedShadowMap();
	CascadedShadowMap(int nSize, float nSlice1 = 0.15, float nSlice2 = 0.3, float nSlice3 = 0.6);

	void buildShadowMaps(Camera *camera, View *view, DirectLight *light);
	void bindShadowMap(int nMap);
	void sendToShader(string sShader);
};

#endif