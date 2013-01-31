#include <stdio.h>
#include "GBuffer.h"
#include "Logger.h"
#include "Profiler.h"
#include "GameState.h"
#include "ShaderManager.h"

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

	// World Position texture
	glGenTextures(1, &m_nWorldPosTex);
	glBindTexture(GL_TEXTURE_2D, m_nWorldPosTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_nWorldPosTex, 0);

	// Glow texture
	glGenTextures(1, &m_nGlowTex);
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, m_nGlowTex, 0);

	// Motion texture
	glGenTextures(1, &m_nMotionTex);
	glBindTexture(GL_TEXTURE_2D, m_nMotionTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, m_nMotionTex, 0);

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

void GBuffer::drawToBuffer(View *view)
{
	Profiler::getInstance()->startProfile("Draw GBuffer");
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("GBuffer");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT, GL_COLOR_ATTACHMENT2_EXT, GL_COLOR_ATTACHMENT3_EXT, GL_COLOR_ATTACHMENT4_EXT};
	glDrawBuffers(5, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	view->use3D(true);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "normalBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 1, "colorBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 2, "positionBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 3, "glowBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 4, "motionBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");
	glBindAttribLocation(glslProgram->getHandle(), 2, "v_normal");
	glBindAttribLocation(glslProgram->getHandle(), 3, "v_tangent");
	glBindAttribLocation(glslProgram->getHandle(), 4, "v_bitangent");

	WorldState *worldState = (WorldState *) GameState::GAMESTATE;
	Camera *camera = worldState->getPhysicsManager()->getWorldCameras()->getCurrentCamera();
	MatrixManager::getInstance()->putMatrix4(PROJECTION, camera->transformToMatrix(MatrixManager::getInstance()->getMatrix4(PROJECTION)));
	glslProgram->sendUniform("projectionCameraMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("camPos",camera->getEyeX(),camera->getEyeY(),camera->getEyeZ());
	
	glslProgram->sendUniform("projectionLastCameraMatrix", &m_m4LastCameraProj[0][0]);
	glslProgram->sendUniform("lastCamPos",m_lastCamera.getEyeX(),m_lastCamera.getEyeY(),m_lastCamera.getEyeZ());

	Frustum *frustum = worldState->getRenderer()->getFrustum();
	worldState->getWorldManager()->renderWorld("GBuffer", frustum);
	glslProgram->disable();
	unbind();

	m_m4LastCameraProj = MatrixManager::getInstance()->getMatrix4(PROJECTION);
	m_lastCamera = *camera;
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

void GBuffer::bindPositionTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nWorldPosTex);
}

void GBuffer::bindGlowTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
}

void GBuffer::bindMotionTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nMotionTex);
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

GLuint GBuffer::getWorldPosTex() 
{
	return m_nWorldPosTex;
}

GLuint GBuffer::getMotionTex() 
{
	return m_nMotionTex;
}

int GBuffer::getWidth() 
{
	return m_nWidth;
}

int GBuffer::getHeight() 
{
	return m_nHeight;
}