#include "GlossyBuffer.h"
#include "Logger.h"
#include "ShaderManager.h"
#include "DrawFunc.h"
#include "VoxelGrid.h"

GlossyBuffer::GlossyBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffersEXT(1,&m_nFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);

	glGenTextures(1, &m_nGlossyTex);
	glBindTexture(GL_TEXTURE_2D, m_nGlossyTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_nGlossyTex, 0);

	// check FbO status
	GLenum FBOstatus = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
	if(FBOstatus != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		Logging::GRAPHICS->error("GL_FRAMEBUFFER_COMPLETE failed, CANNOT use FBO. " + FBOstatus);
	}
	else
	{
		Logging::GRAPHICS->info("Glossy Buffer Done");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

GlossyBuffer::~GlossyBuffer()
{
	glDeleteFramebuffers(1,&m_nFrameBuffer);
}

void GlossyBuffer::drawToBuffer(GLuint nNormalTex, GLuint nDepthTex, GLuint nGlowTex, View *view, Camera *camera)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("GlossyReflection");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT};
	glDrawBuffers(1, mrt);
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

	glBindFragDataLocation(glslProgram->getHandle(), 0, "glossyBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("inverseMVPMatrix", &m4InvMVP[0][0]);
	glslProgram->sendUniform("cameraPos",camera->getEyeX(),camera->getEyeY(),camera->getEyeZ());

	
	glslProgram->sendUniform("worldSize", WORLD_SIZE);
	glslProgram->sendUniform("numVoxels", VOXEL_SIZE);

	glActiveTexture(GL_TEXTURE8);
	glEnable(GL_TEXTURE_3D);
	VoxelGrid::getInstance()->bind(0);
	glActiveTexture(GL_TEXTURE9);
	glEnable(GL_TEXTURE_3D);
	VoxelGrid::getInstance()->bind(1);
	glActiveTexture(GL_TEXTURE10);
	glEnable(GL_TEXTURE_3D);
	VoxelGrid::getInstance()->bind(2);
	glActiveTexture(GL_TEXTURE11);
	glEnable(GL_TEXTURE_3D);
	VoxelGrid::getInstance()->bind(3);
	glslProgram->sendUniform("voxelmap[0]", 8);
	glslProgram->sendUniform("voxelmap[1]", 9);
	glslProgram->sendUniform("voxelmap[2]", 10);
	glslProgram->sendUniform("voxelmap[3]", 11);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, nNormalTex);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, nDepthTex);
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, nGlowTex);
	glslProgram->sendUniform("normalTex",0);
	glslProgram->sendUniform("depthTex",1);
	glslProgram->sendUniform("glowTex",2);

	drawScreenShader(0,0,1.0f,1.0f);
	glslProgram->disable();
	unbind();	
}

void GlossyBuffer::bind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
}

void GlossyBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void GlossyBuffer::bindGlossyTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nGlossyTex);
}

GLuint GlossyBuffer::getGlossyTex() 
{
	return m_nGlossyTex;
}

int GlossyBuffer::getWidth() 
{
	return m_nWidth;
}

int GlossyBuffer::getHeight() 
{
	return m_nHeight;
}