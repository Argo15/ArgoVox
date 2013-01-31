#include "Texture.h"

Texture::Texture()
	: Entity()
{
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);
}

void Texture::remove()
{
	 glDeleteTextures(1,&m_nTextureID);
}