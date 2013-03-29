#ifndef BLURBUFFER_H
#define BLURBUFFER_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <string>
#include "View.h"
using namespace std;

class BlurBuffer
{
private:
	GLuint m_nBlurTex[2];
	GLuint m_nFrameBuffer[2]; // Vertical, Horizontal

	int m_nWidth, m_nHeight;
	void blurPass(int id, string shader, GLuint nTex, GLuint nNormalTex, View *view);

public:
	BlurBuffer(int nWidth, int nHeight);
	~BlurBuffer();

	void drawToBuffer(GLuint nTex, GLuint nNormalTex, View *view);

	void bind(int id);
	void unbind();
	void bindBlurTex();
	GLuint getBlurTex();
	int getWidth();
	int getHeight();
};

#endif