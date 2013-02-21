#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include "View.h"
#include "Camera.h"
#include "DirectLight.h"
#include "VoxelShadowMap.h"

#define VOXEL_SIZE 64
#define WORLD_SIZE 8

class VoxelGrid
{
private:
	VoxelGrid();
	static VoxelGrid* m_pInstance;

	VoxelShadowMap *m_voxelShadowMap;

	GLuint m_nTextureId[6];
	GLuint *m_defaultValues;

	int m_nCurrentMipLevel;

public:
    static VoxelGrid *getInstance();

	void clear();
	void buildVoxels(DirectLight *light);
	void buildVoxels(View *view, Camera *camera, DirectLight *light);
	void bind(GLuint unit, GLuint mipLevel);
	GLuint getTextureId(int mipmap);
	void setMipLevel(int nLevel);
	int getMipLevel();
};

#endif