#include "RandBuffer.h"
#include "Logger.h"
#include "ShaderManager.h"
#include "DrawFunc.h"

RandBuffer::RandBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffersEXT(1,&m_nFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);

	glGenTextures(1, &m_nRandTex);
	glBindTexture(GL_TEXTURE_2D, m_nRandTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_nRandTex, 0);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
	}
	else
	{
		Logging::GRAPHICS->info("Random Buffer Done");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

RandBuffer::~RandBuffer()
{
	glDeleteFramebuffers(1,&m_nFrameBuffer);
}

void RandBuffer::drawToBuffer(View *view)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("Random");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT};
	glDrawBuffers(1, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	view->use3D(false);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "randBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);

	drawScreenShader(0,0,1.0f,1.0f);
	glFinish();
	glslProgram->disable();
	unbind();	
}

void RandBuffer::bind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
}

void RandBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void RandBuffer::bindRandTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nRandTex);
}

GLuint RandBuffer::getRandTex() 
{
	return m_nRandTex;
}

int RandBuffer::getWidth() 
{
	return m_nWidth;
}

int RandBuffer::getHeight() 
{
	return m_nHeight;
}