#ifndef GLOSSYBUFFER_H
#define GLOSSYBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include "View.h"
#include "Camera.h"

class GlossyBuffer 
{
private:
	GLuint m_nGlossyTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	GlossyBuffer(int nWidth, int nHeight);
	~GlossyBuffer();

	void drawToBuffer(GLuint nNormalTex, GLuint nDepthTex, GLuint nGlowTex, View *view, Camera *camera);

	void bind();
	void unbind();
	void bindGlossyTex();
	GLuint getGlossyTex();
	int getWidth();
	int getHeight();
};

#endif