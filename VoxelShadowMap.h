#ifndef VOXEL_SHADOW_MAP
#define VOXEL_SHADOW_MAP

#include "DepthBuffer.h"
#include "DirectLight.h"
#include "Matrix.h"
#include "ShaderManager.h"
#include "SceneManager.h"

class VoxelShadowMap
{
private:
	DepthBuffer *m_shadowMap;
	glm::mat4 m_m4LightMatrix;
	int m_nSize;

public:
	VoxelShadowMap();
	VoxelShadowMap(int nSize);

	void buildShadowMap(DirectLight *light);
	void sendToShader(string sShader);

};

#endif