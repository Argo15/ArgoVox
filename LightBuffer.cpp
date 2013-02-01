#include "LightBuffer.h"
#include "DrawFunc.h"
#include <assert.h>
#include "Logger.h"
#include "Profiler.h"
#include "GameState.h"
#include "ShaderManager.h"

LightBuffer::LightBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffersEXT(1,&m_nFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);

	glGenTextures(1, &m_nLightTex);
	glBindTexture(GL_TEXTURE_2D, m_nLightTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_nLightTex, 0);

	glGenTextures(1, &m_nGlowTex);
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_TEXTURE_2D, m_nGlowTex, 0);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
	}
	else
	{
		Logging::GRAPHICS->info("Light Buffer Done");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	light = new DirectLight();
	shadowMap = new CascadedShadowMap(1024);
}

LightBuffer::~LightBuffer()
{
	glDeleteFramebuffers(1,&m_nFrameBuffer);
}

void LightBuffer::drawToBuffer(GLuint nNormalTex, GLuint nDepthTex, GLuint nGlowTex, View *view, Camera *camera)
{
	shadowMap->buildShadowMaps(camera, view, light);

	Profiler::getInstance()->startProfile("Draw Light");
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("DirectLight");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
	glDrawBuffers(2, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	camera->transform();
	view->use3D(true);
	glm::mat4 m4InvMVP = MatrixManager::getInstance()->getMatrix4(PROJECTION) * MatrixManager::getInstance()->getMatrix4(MODELVIEW);
	m4InvMVP = glm::inverse(m4InvMVP);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	view->use3D(false);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "lightBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 1, "glowBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");

	light->sendToShader("DirectLight");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("inverseMVPMatrix", &m4InvMVP[0][0]);
	glslProgram->sendUniform("near", (float)view->getNear());
	glslProgram->sendUniform("far", (float)view->getFar());
	glslProgram->sendUniform("cameraPos",camera->getEyeX(),camera->getEyeY(),camera->getEyeZ());

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, nNormalTex);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, nDepthTex);
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, nGlowTex);
	glslProgram->sendUniform("normalTex",0);
	glslProgram->sendUniform("depthTex",1);
	glslProgram->sendUniform("glowTex",2);

	shadowMap->sendToShader("DirectLight");

	drawScreenShader(0,0,1.0f,1.0f);
	glslProgram->disable();
	unbind();	
	Profiler::getInstance()->endProfile();
}

void LightBuffer::bind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
}

void LightBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void LightBuffer::bindLightTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nLightTex);
}

void LightBuffer::bindGlowTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
}

GLuint LightBuffer::getLightTex() 
{
	return m_nLightTex;
}

GLuint LightBuffer::getGlowTex() 
{
	return m_nGlowTex;
}

int LightBuffer::getWidth() 
{
	return m_nWidth;
}

int LightBuffer::getHeight() 
{
	return m_nHeight;
}