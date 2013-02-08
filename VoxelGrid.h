#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#include "View.h"
#include "Camera.h"

#define VOXEL_SIZE 128
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
	void buildVoxels(View *view, Camera *camera);
	void bind(GLuint unit);
	GLuint getTextureId();
};

#endif