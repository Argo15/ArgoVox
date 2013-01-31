#include "AtmosphereBuffer.h"
#include "DrawFunc.h"
#include <assert.h>
#include "Logger.h"
#include "Profiler.h"
#include "GameState.h"
#include "MatrixManager.h"
#include "ShaderManager.h"
#include "TextureManager.h"

static Vector3 DEFAULT_SKY_NEAR_COLOR(77, 153, 204);
static Vector3 DEFAULT_SKY_AWAY_COLOR(26, 52, 204);
static Vector3 DEFAULT_CLOUD_COLOR(255, 255, 255);

AtmosphereBuffer::AtmosphereBuffer(int nWidth, int nHeight)
{
	this->m_nWidth=nWidth;
	this->m_nHeight=nHeight;

	glEnable(GL_TEXTURE_2D);

	glGenFramebuffersEXT(1,&m_nFrameBuffer);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);

	glGenTextures(1, &m_nColorTex);
	glBindTexture(GL_TEXTURE_2D, m_nColorTex);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_nWidth, m_nHeight, 0, GL_RGBA, GL_FLOAT, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, m_nColorTex, 0);

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
		Logging::GRAPHICS->info("Atmosphere Buffer Done");
	}

	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

AtmosphereBuffer::~AtmosphereBuffer()
{
	glDeleteFramebuffers(1,&m_nFrameBuffer);
}

void AtmosphereBuffer::drawToBuffer(GLuint colorBuf, GLuint glowBuf, GLuint depthBuf, View *view)
{
	Profiler::getInstance()->startProfile("Draw Atmosphere");
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("Atmosphere");
	glslProgram->use();

	bind();
	GLenum mrt[] = {GL_COLOR_ATTACHMENT0_EXT, GL_COLOR_ATTACHMENT1_EXT};
	glDrawBuffers(2, mrt);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushAttrib( GL_VIEWPORT_BIT );
	glViewport( 0, 0, getWidth(), getHeight());

	WorldState *worldState = (WorldState *) GameState::GAMESTATE;

	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f)); 
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f)); 
	Camera *camera = worldState->getPhysicsManager()->getWorldCameras()->getCurrentCamera();
	camera->transform();
	view->use3D(true);
	glm::mat4 m4InvMVP = MatrixManager::getInstance()->getMatrix4(PROJECTION) * MatrixManager::getInstance()->getMatrix4(MODELVIEW);
	m4InvMVP = glm::inverse(m4InvMVP);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	view->use3D(false);

	glBindFragDataLocation(glslProgram->getHandle(), 0, "colorBuffer");
	glBindFragDataLocation(glslProgram->getHandle(), 1, "glowBuffer");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");
	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("inverseMVPMatrix", &m4InvMVP[0][0]);
	glslProgram->sendUniform("cameraPos",camera->getEyeX(),camera->getEyeY(),camera->getEyeZ());
	Vector3 vSunDir = worldState->getWorldManager()->getSun()->getDirection();
	glslProgram->sendUniform("sunDir",vSunDir[0],vSunDir[1],vSunDir[2]);
	glslProgram->sendUniform("nearSunColor",getNearSunColor()[0],getNearSunColor()[1],getNearSunColor()[2]);
	glslProgram->sendUniform("awaySunColor",getAwaySunColor()[0],getAwaySunColor()[1],getAwaySunColor()[2]);
	glslProgram->sendUniform("cloudColor",getCloudColor()[0],getCloudColor()[1],getCloudColor()[2]);

	glActiveTexture(GL_TEXTURE0); 
	glBindTexture(GL_TEXTURE_2D, colorBuf);
	glActiveTexture(GL_TEXTURE1); 
	glBindTexture(GL_TEXTURE_2D, glowBuf);
	glActiveTexture(GL_TEXTURE2); 
	glBindTexture(GL_TEXTURE_2D, depthBuf);
	glActiveTexture(GL_TEXTURE3); 
	TextureManager::getInstance()->bindTexture("Cloud");
	glslProgram->sendUniform("colorTex",0);
	glslProgram->sendUniform("glowTex",1);
	glslProgram->sendUniform("depthTex",2);
	glslProgram->sendUniform("cloudTex",3);
	glslProgram->sendUniform("near", (float)view->getNear());
	glslProgram->sendUniform("far", (float)view->getFar());

	drawScreenShader(0,0,1.0f,1.0f);
	glslProgram->disable();
	unbind();	
	Profiler::getInstance()->endProfile();
}

Vector3 AtmosphereBuffer::getNearSunColor()
{
	float nTimeWeights[] = {
		getWeightForTimeOfDay(TimeUtils::MORNING),
		getWeightForTimeOfDay(TimeUtils::DAY),
		getWeightForTimeOfDay(TimeUtils::EVENING),
		getWeightForTimeOfDay(TimeUtils::NIGHT)
	};
	Vector3 vTimeColors[] = {
		getSkyNearSunColor(TimeUtils::MORNING),
		getSkyNearSunColor(TimeUtils::DAY),
		getSkyNearSunColor(TimeUtils::EVENING),
		getSkyNearSunColor(TimeUtils::NIGHT)
	};
	Vector3 vColor(0);
	for (int i=0; i<4; i++)
	{
		vColor = vColor + vTimeColors[i] * nTimeWeights[i];
	}
	return vColor;
}

Vector3 AtmosphereBuffer::getAwaySunColor()
{
	float nTimeWeights[] = {
		getWeightForTimeOfDay(TimeUtils::MORNING),
		getWeightForTimeOfDay(TimeUtils::DAY),
		getWeightForTimeOfDay(TimeUtils::EVENING),
		getWeightForTimeOfDay(TimeUtils::NIGHT)
	};
	Vector3 vTimeColors[] = {
		getSkyAwaySunColor(TimeUtils::MORNING),
		getSkyAwaySunColor(TimeUtils::DAY),
		getSkyAwaySunColor(TimeUtils::EVENING),
		getSkyAwaySunColor(TimeUtils::NIGHT)
	};
	Vector3 vColor(0);
	for (int i=0; i<4; i++)
	{
		vColor = vColor + vTimeColors[i] * nTimeWeights[i];
	}
	return vColor;
}

Vector3 AtmosphereBuffer::getCloudColor()
{
	float nTimeWeights[] = {
		getWeightForTimeOfDay(TimeUtils::MORNING),
		getWeightForTimeOfDay(TimeUtils::DAY),
		getWeightForTimeOfDay(TimeUtils::EVENING),
		getWeightForTimeOfDay(TimeUtils::NIGHT)
	};
	Vector3 vTimeColors[] = {
		TimeUtils::getCloudColor(TimeUtils::MORNING),
		TimeUtils::getCloudColor(TimeUtils::DAY),
		TimeUtils::getCloudColor(TimeUtils::EVENING),
		TimeUtils::getCloudColor(TimeUtils::NIGHT)
	};
	Vector3 vColor(0);
	for (int i=0; i<4; i++)
	{
		vColor = vColor + vTimeColors[i] * nTimeWeights[i];
	}
	return vColor;
}

void AtmosphereBuffer::bind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_nFrameBuffer);
}
	
void AtmosphereBuffer::unbind() 
{
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
}

void AtmosphereBuffer::bindColorTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nColorTex);
}
	
void AtmosphereBuffer::bindGlowTex() 
{
	glBindTexture(GL_TEXTURE_2D, m_nGlowTex);
}

GLuint AtmosphereBuffer::getGlowTex() 
{
	return m_nGlowTex;
}
	
GLuint AtmosphereBuffer::getColorTex() 
{
	return m_nColorTex;
}

int AtmosphereBuffer::getWidth() 
{
	return m_nWidth;
}
	
int AtmosphereBuffer::getHeight() 
{
	return m_nHeight;
}