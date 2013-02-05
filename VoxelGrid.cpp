#include <gl/glew.h>
#include "VoxelGrid.h"

VoxelGrid::VoxelGrid(int nWorldSize)
{
	glewInit();
	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &m_nTextureId);
	glBindTexture(GL_TEXTURE_3D, m_nTextureId);
	GLuint texels[VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE];
	for (int r=0; r<VOXEL_SIZE; r++)
	{
		for (int g=0; g<VOXEL_SIZE; g++)
		{
			for (int b=0; b<VOXEL_SIZE; b++)
			{
				texels[r + VOXEL_SIZE*g + VOXEL_SIZE*VOXEL_SIZE*b] = ((GLubyte)(255*(r+1)/VOXEL_SIZE)) & 0x000000FF;
				texels[r + VOXEL_SIZE*g + VOXEL_SIZE*VOXEL_SIZE*b] |= (((GLubyte)(255*(g+1)/VOXEL_SIZE)) & 0x000000FF) << 8U;
				texels[r + VOXEL_SIZE*g + VOXEL_SIZE*VOXEL_SIZE*b] |= (((GLubyte)(255*(b+1)/VOXEL_SIZE)) & 0x000000FF) << 16U;
				texels[r + VOXEL_SIZE*g + VOXEL_SIZE*VOXEL_SIZE*b] |= (((GLubyte)(255))&0x000000FF) << 24U;
			}
		}
	}
	GLenum error = glGetError();
	glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_R32UI, VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, texels);
	error = glGetError();
	glActiveTextureARB(GL_TEXTURE0);
}

void VoxelGrid::bind(GLuint unit)
{
	glBindImageTexture(unit, m_nTextureId, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
}
	
GLuint VoxelGrid::getTextureId()
{
	return m_nTextureId;
}