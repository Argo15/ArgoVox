#ifndef TEXTURE_H
#define TEXTURE_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/glee.h>
#include "Entity.h"

class Texture : public Entity
{
protected:
	GLuint m_nTextureID;

public:
	Texture();

	virtual bool load(const char *sFilename) = 0;
	void use();
	void remove();
};

#endif