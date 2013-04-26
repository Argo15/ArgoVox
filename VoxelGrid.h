#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include "View.h"
#include "Camera.h"
#include "DirectLight.h"
#include "VoxelShadowMap.h"

#define VOXEL_SIZE 64
#define HASHMAP_SIZE 256
#define HASHING_PRIME 59999
#define WORLD_SIZE 8

class VoxelGrid
{
private:
	VoxelGrid();
	static VoxelGrid* m_pInstance;

	VoxelShadowMap *m_voxelShadowMap;

	GLuint m_nTextureId[4];
	GLuint m_nPositionMap[4];
	char *m_defaultValues;
	float *m_defaultPos;

	int m_nCurrentMipLevel;

public:
    static VoxelGrid *getInstance();

	void clear();
	void buildVoxels(DirectLight *light);
	void buildVoxels(View *view, Camera *camera, DirectLight *light);
	void buildMipmap(int mipLevel);
	void bind(GLuint mipLevel);
	void bindPos(GLuint mipLevel);
	void bind(GLuint gridUnit, GLuint posUnit, GLuint mipLevel, GLenum access = GL_READ_WRITE);
	GLuint getTextureId(int mipmap);
	void setMipLevel(int nLevel);
	int getMipLevel();
};

#endif