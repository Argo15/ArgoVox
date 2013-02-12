#ifndef LIGHTBUFFER_H
#define LIGHTBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <vector>
#include "View.h"
#include "Texture.h"
#include "Matrix.h"
#include "GLSLProgram.h"
#include "Camera.h"
#include "WorldState.h"
#include "DirectLight.h"
#include "CascadedShadowMap.h"
using namespace std;

class CascadedShadowMap;

class LightBuffer 
{
private:
	GLuint m_nLightTex;
	GLuint m_nGlowTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

	DirectLight *light;
	CascadedShadowMap *shadowMap;

public:
	LightBuffer(int nWidth, int nHeight);
	~LightBuffer();

	void drawToBuffer(GLuint nNormalTex, GLuint nDepthTex, GLuint nGlowTex, View *view, Camera *camera);

	void bind();
	void unbind();
	void bindLightTex();
	void bindGlowTex();
	GLuint getLightTex();
	GLuint getGlowTex();
	int getWidth();
	int getHeight();
	DirectLight *getLight();

};

#endif