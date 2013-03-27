#include <gl/glew.h>
#include "VoxelGrid.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "DrawFunc.h"
#include "Profiler.h"

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
	glGenTextures(4, m_nTextureId);
	m_defaultValues = new char[VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE*4];
	for (int i=0; i<VOXEL_SIZE*VOXEL_SIZE*VOXEL_SIZE*4; i++)
	{
		m_defaultValues[i] = (char)0.0f;
	}
	for (int i=0; i<4; i++)
	{
		glBindTexture(GL_TEXTURE_3D, m_nTextureId[i]);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_defaultValues);
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
		glTexImage3D(GL_TEXTURE_3D, 0, GL_RGBA8, VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), VOXEL_SIZE / pow(2.0, i), 0, GL_RGBA, GL_UNSIGNED_BYTE, m_defaultValues);
	}
	glGenerateMipmap(GL_TEXTURE_3D);
	glActiveTextureARB(GL_TEXTURE0);
}

void VoxelGrid::buildVoxels(DirectLight *light)
{
	Profiler::getInstance()->startProfile("Build Voxels");
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
	
	buildMipmap(1);
	buildMipmap(2);
	buildMipmap(3);
	glFinish();
	Profiler::getInstance()->endProfile();
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

void VoxelGrid::buildMipmap(int mipLevel)
{
	int mipFactor = (int)pow(2.0, (double)mipLevel);
	int nVoxelSize = VOXEL_SIZE / mipFactor;
	
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("BuildMipmap");
	glslProgram->use();

	glClear(GL_DEPTH_BUFFER_BIT);
	
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));

	View *view = new View();
	view->viewport(0, 0, nVoxelSize, nVoxelSize);
	view->set2D(0, 1.0, 0, 1.0, 0, 1.0);
	view->use3D(false);
	
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("numVoxels", nVoxelSize);

	this->bind(0, mipLevel-1, GL_READ_ONLY);
	this->bind(1, mipLevel, GL_WRITE_ONLY);

	drawScreenShader(0,0,1.0f,1.0f);

	glslProgram->disable();
}

void VoxelGrid::bind(GLuint mipLevel)
{
	glBindTexture(GL_TEXTURE_3D, m_nTextureId[mipLevel]);
}

void VoxelGrid::bind(GLuint unit, GLuint mipLevel, GLenum access)
{
	glBindImageTexture(unit, m_nTextureId[mipLevel], 0, GL_TRUE, 0, access, GL_RGBA8);
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