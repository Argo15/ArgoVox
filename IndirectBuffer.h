#ifndef INDIRECTBUFFER_H
#define INDIRECTBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include "View.h"
#include "Camera.h"

class IndirectBuffer 
{
private:
	GLuint m_nIndirectTex;
	GLuint m_nFrameBuffer;

	int m_nWidth, m_nHeight;

public:
	IndirectBuffer(int nWidth, int nHeight);
	~IndirectBuffer();

	void drawToBuffer(GLuint nDepthTex, GLuint nTangentTex, GLuint nBiangentTex, GLuint nNormalTex, GLuint nDiffuseTex, View *view, Camera *camera);

	void bind();
	void unbind();
	void bindIndirectTex();
	GLuint getIndirectTex();
	int getWidth();
	int getHeight();
};

#endif