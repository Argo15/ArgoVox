#ifndef ATMOSPHEREBUFFER_H
#define ATMOSPHEREBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <vector>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include "Chunk.h"
#include "WorldState.h"
#include "Camera.h"
#include "Frustum.h"
#include "View.h"
#include "TimeManager.h"
#include "Config.h"
#include "Matrix.h"
#include "MathUtils.h"
#include "TimeUtils.h"
using namespace std;

class AtmosphereBuffer
{
private:
	GLuint m_nColorTex;
	GLuint m_nGlowTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	AtmosphereBuffer(int nWidth, int nHeight);
	~AtmosphereBuffer();

	void drawToBuffer(GLuint colorBuf, GLuint glowBuf, GLuint depthBuf, View *view);
	void bind();
	void unbind();
	void bindColorTex();
	void bindGlowTex();
	GLuint getGlowTex();
	GLuint getColorTex();
	int getWidth();
	int getHeight();
	
	Vector3 getNearSunColor();
	Vector3 getAwaySunColor();
	Vector3 getCloudColor();
};

#endif