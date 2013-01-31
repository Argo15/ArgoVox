#include <stdio.h>
#include "DepthBuffer.h"
#include "Logger.h"

DepthBuffer::DepthBuffer(int nWidth, int nHeight)
{
	glEnable(GL_TEXTURE_2D);

	// Generate shadow map Texture
	glGenTextures(1, &m_nTexture);
	glBindTexture(GL_TEXTURE_2D, m_nTexture);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, nWidth, nHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_R_TO_TEXTURE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
	glTexParameteri( GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create FbO
	glGenFramebuffersEXT(1,&m_FrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT,GL_TEXTURE_2D, m_nTexture, 0);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
	}
	else
	{
		Logging::GRAPHICS->info("Depth Buffer Done");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void DepthBuffer::bind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_FrameBuffer);
}
	
void DepthBuffer::unbind()
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}
	
GLuint DepthBuffer::getBuffer()
{
	return m_FrameBuffer;
}
	
GLuint DepthBuffer::getTexture()
{
	return m_nTexture;
}