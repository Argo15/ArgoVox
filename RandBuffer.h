#ifndef RANDBUFFER_H
#define RANDBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include "View.h"

class RandBuffer 
{
private:
	GLuint m_nRandTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	RandBuffer(int nWidth, int nHeight);
	~RandBuffer();

	void drawToBuffer(View *view);

	void bind();
	void unbind();
	void bindRandTex();
	GLuint getRandTex();
	int getWidth();
	int getHeight();
};

#endif