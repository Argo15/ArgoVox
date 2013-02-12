#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include "View.h"
#include "Camera.h"
#include "DirectLight.h"

#define VOXEL_SIZE 64
#define WORLD_SIZE 16

class VoxelGrid
{
private:
	VoxelGrid();
	static VoxelGrid* m_pInstance;

	GLuint m_nTextureId;
	GLuint *m_defaultValues;

public:
    static VoxelGrid *getInstance();

	void clear();
	void buildVoxels(DirectLight *light);
	void buildVoxels(View *view, Camera *camera, DirectLight *light);
	void bind(GLuint unit);
	GLuint getTextureId();
};

#endif