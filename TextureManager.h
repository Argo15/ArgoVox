#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <string>
#include <map>
#include "TgaTexture.h"
#include "PerlinNoise.h"
using namespace std;

class TextureManager 
{
private:
	static TextureManager *m_pInstance;
	map<string,Texture *> m_textures;

	TextureManager();
public:
	static TextureManager *getInstance();

    void initialize();
	void loadTexture(string sFileName);
	void bindTexture(string sName);
	void deleteTexture(string sName);
	void deleteAllTextures();

	Texture *getTexture(string sName);
	map<string,Texture *> *getTextures();
};

#endif