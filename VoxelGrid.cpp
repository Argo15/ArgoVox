#include <gl/glew.h>
#include "VoxelGrid.h"
#include "ShaderManager.h"
#include "SceneManager.h"

VoxelGrid* VoxelGrid::m_pInstance = 0;

VoxelGrid *VoxelGrid::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new VoxelGrid();
	}
	return m_pInstance;
}

VoxelGrid::VoxelGrid()
{
	glewInit();
	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_3D);
	glGenTextures(1, &m_nTextureId);
	glBindTexture(GL_TEXTURE_3D, m_nTextureId);
	m_defaultValues = new GLuint[VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE];
	for (int i=0; i<VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE; i++)
	{
		m_defaultValues[i] = 0;
	}
	glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_R32UI, VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_defaultValues);
	glActiveTextureARB(GL_TEXTURE0);
}

void VoxelGrid::clear()
{
	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_3D);
	glBindTexture(GL_TEXTURE_3D, m_nTextureId);
	glTexImage3DEXT(GL_TEXTURE_3D, 0, GL_R32UI, VOXEL_SIZE, VOXEL_SIZE, VOXEL_SIZE, 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_defaultValues);
	glActiveTextureARB(GL_TEXTURE0);
}

void VoxelGrid::buildVoxels(View *view, Camera *camera)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, 8*VOXEL_SIZE, 8*VOXEL_SIZE);

	Camera newCamera;
	newCamera.setPosition(WORLD_SIZE,WORLD_SIZE,WORLD_SIZE);
	newCamera.setLookAt(0,0,0);
	newCamera.setUp(0,1.0f,0);

	view->use3D(true);
	newCamera.transform();

	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("BuildVoxels");
	glslProgram->use();

	glBindFragDataLocation(glslProgram->getHandle(), 0, "fragColor");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");
	glBindAttribLocation(glslProgram->getHandle(), 2, "v_normal");
	glBindAttribLocation(glslProgram->getHandle(), 3, "v_tangent");
	glBindAttribLocation(glslProgram->getHandle(), 4, "v_bitangent");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	glm::mat4 cameraInverse = glm::mat4(1.0);
	cameraInverse = newCamera.transformToMatrix(cameraInverse);
	cameraInverse = glm::inverse(cameraInverse);
	glslProgram->sendUniform("invCameraMatrix", &cameraInverse[0][0]);

	this->bind(3);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SceneManager::getInstance()->draw("BuildVoxels");

	glslProgram->disable();
	glPopAttrib( );
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void VoxelGrid::bind(GLuint unit)
{
	glBindImageTexture(unit, m_nTextureId, 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
}
	
GLuint VoxelGrid::getTextureId()
{
	return m_nTextureId;
}