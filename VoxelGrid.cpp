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
	glGenTextures(6, m_nTextureId);
	m_defaultValues = new GLuint[VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE];
	for (int i=0; i<VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE; i++)
	{
		m_defaultValues[i] = 0;
	}
	for (int i=0; i<4; i++)
	{
		glBindTexture(GL_TEXTURE_3D, m_nTextureId[i]);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32UI, VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_defaultValues);
	}

	glActiveTextureARB(GL_TEXTURE0);

	m_voxelShadowMap = new VoxelShadowMap(512);
	m_nCurrentMipLevel = 0;
}

void VoxelGrid::clear()
{
	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_3D);
	for (int i=0; i<4; i++)
	{
		glBindTexture(GL_TEXTURE_3D, m_nTextureId[i]);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_R32UI, VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), 0, GL_RED_INTEGER, GL_UNSIGNED_INT, m_defaultValues);	
	}
	glGenerateMipmap(GL_TEXTURE_3D);
	glActiveTextureARB(GL_TEXTURE0);
}

void VoxelGrid::buildVoxels(DirectLight *light)
{
	clear();

	m_voxelShadowMap->buildShadowMap(light);

	View *view = new View();
	view->viewport(0, 0, VOXEL_SIZE, VOXEL_SIZE);
	view->set2D(-WORLD_SIZE/2.0, WORLD_SIZE/2.0, -WORLD_SIZE/2.0, WORLD_SIZE/2.0, 0, WORLD_SIZE);
	Camera *camera = new Camera();
	camera->setPosition(0,0,WORLD_SIZE/2.0);
	camera->setLookAt(0,0,0);
	camera->setUp(0,1.0f,0);
	buildVoxels(view, camera, light);
	camera->setPosition(0,WORLD_SIZE/2.0,0);
	camera->setLookAt(0,0,0);
	camera->setUp(0,0,1.0f);
	buildVoxels(view, camera, light);
	camera->setPosition(WORLD_SIZE/2.0,0,0);
	camera->setLookAt(0,0,0);
	camera->setUp(0,1.0f,0);
	buildVoxels(view, camera, light);
}

void VoxelGrid::buildVoxels(View *view, Camera *camera, DirectLight *light)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	glPushAttrib( GL_VIEWPORT_BIT );
	view->viewport();

	view->use3D(false);
	camera->transform();

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
	cameraInverse = camera->transformToMatrix(cameraInverse);
	cameraInverse = glm::inverse(cameraInverse);
	glslProgram->sendUniform("invCameraMatrix", &cameraInverse[0][0]);
	glslProgram->sendUniform("voxelGridSize", WORLD_SIZE);
	glslProgram->sendUniform("numVoxels", VOXEL_SIZE);

	light->sendToShader("BuildVoxels");

	m_voxelShadowMap->sendToShader("BuildVoxels");

	this->bind(0, 0);
	this->bind(1, 1);
	this->bind(2, 2);
	this->bind(3, 3);
	//this->bind(4, 4);
	//this->bind(5, 5);

	glDisable(GL_DEPTH_TEST);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	SceneManager::getInstance()->draw("BuildVoxels");

	glslProgram->disable();
	glPopAttrib( );
	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
}

void VoxelGrid::bind(GLuint unit, GLuint mipLevel)
{
	glBindImageTexture(unit, m_nTextureId[mipLevel], 0, GL_TRUE, 0, GL_READ_WRITE, GL_R32UI);
}
	
GLuint VoxelGrid::getTextureId(int mipmap)
{
	return m_nTextureId[mipmap];
}

void VoxelGrid::setMipLevel(int nLevel)
{
	m_nCurrentMipLevel = nLevel;
}
	
int VoxelGrid::getMipLevel()
{
	return m_nCurrentMipLevel;
}