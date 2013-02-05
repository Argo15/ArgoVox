#include <stdio.h>
#include "GBuffer.h"
#include "Logger.h"
#include "Profiler.h"
#include "GameState.h"
#include "ShaderManager.h"
#include "SceneManager.h"
#include "MaterialManager.h"

GBuffer::GBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffers(1,&m_nFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_nFrameBuffer);

	// Generate depth texture
	glGenTextures(1, &m_nDepthTex);
	glBindTexture(GL_TEXTURE_2D, m_nDepthTex);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, m_nWidth, m_nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0 );
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_nDepthTex, 0);

	// Generate normal
	glGenTextures(1, &m_nNormalTex);
	glBindTexture(GL_TEXTURE_2D, m_nNormalTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_nNormalTex, 0);

	// Generate color
	glGenTextures(1, &m_nColorTex);
	glBindTexture(GL_TEXTURE_2D, m_nColorTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_nColorTex, 0);

	// Glow texture
	glGenTextures(1, &m_nGlowTex);
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_nGlowTex, 0);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE)
	{
		Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
	}
	else
	{
		Logging::GRAPHICS->info("gBuffer Done");
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer()
{
	glDeleteFramebuffers(1,&m_nFrameBuffer);
}

void GBuffer::drawToBuffer(View *view, Camera *camera, Grid *myGrid, VoxelGrid *voxelGrid)
{
	Profiler::getInstance()->startProfile("Draw GBuffer");
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("GBuffer");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT};
	glDrawBuffers(3, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	view->use3D(true);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "normalBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 1, "colorBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 3, "glowBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");
	glBindAttribLocation(glslProgram->getHandle(), 2, "v_normal");
	glBindAttribLocation(glslProgram->getHandle(), 3, "v_tangent");
	glBindAttribLocation(glslProgram->getHandle(), 4, "v_bitangent");

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, camera->transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
	glm::mat4 m4InvCamera = MatrixManager::getInstance()->getMatrix4(MODELVIEW);
	m4InvCamera = glm::inverse(m4InvCamera);
	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	glslProgram->sendUniform("invCameraMatrix", &m4InvCamera[0][0]);

	voxelGrid->bind(3);

	MaterialManager::getInstance()->getMaterial("Default")->sendToShader("GBuffer");
	myGrid->draw();

	SceneManager::getInstance()->draw("GBuffer");
	glslProgram->disable();

	MaterialManager::getInstance()->getMaterial("Default")->sendToShader("GBuffer");
	SceneManager::getInstance()->drawTransformers();

	unbind();
	Profiler::getInstance()->endProfile();
}

void GBuffer::bind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
}

void GBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void GBuffer::bindDepthTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nDepthTex);
}

void GBuffer::bindNormalTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nNormalTex);
}

void GBuffer::bindColorTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nColorTex);
}

void GBuffer::bindGlowTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
}

GLuint GBuffer::getGlowTex() 
{
	return m_nGlowTex;
}

GLuint GBuffer::getDepthTex() 
{
	return m_nDepthTex;
}

GLuint GBuffer::getColorTex() 
{
	return m_nColorTex;
}

GLuint GBuffer::getNormalTex() 
{
	return m_nNormalTex;
}

int GBuffer::getWidth() 
{
	return m_nWidth;
}

int GBuffer::getHeight() 
{
	return m_nHeight;
}