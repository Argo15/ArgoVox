#ifndef COLORBUFFER_H
#define COLORBUFFER_H

#include <windows.h>
#include <gl/glee.h>

class ColorBuffer
{
protected:
	int width,height;
	GLuint texture;
	GLuint buffer;

public:
	ColorBuffer(int width, int height);
	~ColorBuffer() {glDeleteFramebuffers(1,&buffer);}
	void bind() {glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, buffer);}
	void unbind() {glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);}
	GLuint getBuffer() {return buffer;}
	GLuint getTexture() {return texture;}
	void bindTexture() {glBindTexture(GL_TEXTURE_2D, texture);}

	int getWidth() {return width;}
	int getHeight() {return height;}
};
#endif