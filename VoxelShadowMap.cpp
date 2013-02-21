#include "VoxelShadowMap.h"
#include "VoxelGrid.h"


VoxelShadowMap::VoxelShadowMap()
{
	VoxelShadowMap(1024);
}

VoxelShadowMap::VoxelShadowMap(int nSize)
{
	m_nSize = nSize;
	m_shadowMap = new DepthBuffer(m_nSize, m_nSize);
}


void VoxelShadowMap::buildShadowMap(DirectLight *light)
{
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);

	Vector3 lightDir = light->getDirection();
	lightDir = lightDir.normalize();
	Camera *lightCamera = new Camera();
	lightCamera->setLookAt(0,0,0);
	lightCamera->setUp(0,1.0f,0);
	lightCamera->setPosition(-lightDir[0]*WORLD_SIZE*2, -lightDir[1]*WORLD_SIZE*2, -lightDir[2]*WORLD_SIZE*2);
	View *lightView = new View();
	lightView->set2D(-WORLD_SIZE*2, WORLD_SIZE*2, -WORLD_SIZE*2, WORLD_SIZE*2, 0, WORLD_SIZE*8);

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
		
	m_shadowMap->bind();
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glClear(GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, m_nSize, m_nSize);
	lightView->use3D(false);

	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("DirectShadow");
	glslProgram->use();
		
	glm::mat4 cameraMat = glm::mat4(1.0f);
	cameraMat = lightCamera->transformToMatrix(cameraMat);

	m_m4LightMatrix = MatrixManager::getInstance()->getMatrix4(PROJECTION) * cameraMat;
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, cameraMat);

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");

	SceneManager::getInstance()->draw("DirectShadow");
	glslProgram->disable();

	glPopAttrib();
	m_shadowMap->unbind();
	
	m_m4LightMatrix = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * m_m4LightMatrix;
}

void VoxelShadowMap::sendToShader(string sShader)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(sShader);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,m_shadowMap->getTexture());
	glslProgram->sendUniform("shadowMap",4);
	glslProgram->sendUniform("lightMatrix", &m_m4LightMatrix[0][0]);
}