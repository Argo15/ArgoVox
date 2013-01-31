#ifndef MOTIONBLURBUFFER_H
#define MOTIONBLURBUFFER_H

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
using namespace std;

class MotionBlurBuffer
{
private:
	GLuint m_nBlurTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	MotionBlurBuffer(int nWidth, int nHeight);
	~MotionBlurBuffer();

	void drawToBuffer(GLuint nTexture, GLuint nVelocityTex, int nNumSamples, View *view);

	void bind();
	void unbind();
	void bindBlurTex();
	GLuint getBlurTex();
	int getWidth();
	int getHeight();
};

#endif