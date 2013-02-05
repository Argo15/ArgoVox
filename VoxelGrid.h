#ifndef VOXEL_GRID_H
#define VOXEL_GRID_H

#define VOXEL_SIZE 32

class VoxelGrid
{
private:
	GLuint m_nTextureId;

public:
	VoxelGrid(int nWorldSize);

	void bind(GLuint unit);
	GLuint getTextureId();
};

#endif