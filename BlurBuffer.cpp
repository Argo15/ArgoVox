#include "BlurBuffer.h"
#include "Logger.h"
#include "ShaderManager.h"
#include "DrawFunc.h"

#define HORIZONTAL 0
#define VERTICAL 1

BlurBuffer::BlurBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffersEXT(2,m_nFrameBuffer);
	glGenTextures(2, m_nBlurTex);
	for (int i=0; i<2; i++)
	{
		glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer[i]);

		glBindTexture(GL_TEXTURE_2D, m_nBlurTex[i]);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_nBlurTex[i], 0);

		// check FbO status
		GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
		if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
		{
			Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
		}
		else
		{
			Logging::GRAPHICS->info("Blur Buffer Done");
		}
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

BlurBuffer::~BlurBuffer()
{
	glDeleteFramebuffers(2,m_nFrameBuffer);
}

void BlurBuffer::blurPass(int id, string shader, GLuint nTex, View *view)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(shader);
	glslProgram->use();

	bind(id);
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT};
	glDrawBuffers(1, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	view->use3D(false);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "blurBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, nTex);
	glslProgram->sendUniform("bluringTex",0);

	drawScreenShader(0,0,1.0f,1.0f);
	glslProgram->disable();
	unbind();
}

void BlurBuffer::drawToBuffer(GLuint nTex, View *view)
{
	blurPass(HORIZONTAL, "HorizontalBlur", nTex, view);
	blurPass(VERTICAL, "VerticalBlur", m_nBlurTex[HORIZONTAL], view);
}

void BlurBuffer::bind(int id) 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer[id]);
}

void BlurBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void BlurBuffer::bindBlurTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nBlurTex[VERTICAL]);
}

GLuint BlurBuffer::getBlurTex() 
{
	return m_nBlurTex[VERTICAL];
}

int BlurBuffer::getWidth() 
{
	return m_nWidth;
}

int BlurBuffer::getHeight() 
{
	return m_nHeight;
}