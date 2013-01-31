#include "TextureManager.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include "dirent.h"
#include "Logger.h"

TextureManager* TextureManager::m_pInstance = 0;

TextureManager::TextureManager()
{
	
}

TextureManager *TextureManager::getInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new TextureManager();
	}
	return m_pInstance;
}

void TextureManager::initialize()
{
	Logging::GRAPHICS->info("Loading Textures");
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("./Data/Textures") )
	{
		while(entry = readdir(pDIR))
		{
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				loadTexture(string("./Data/Textures/") + entry->d_name);
			}
		}
		closedir(pDIR);
	}
	m_textures["Cloud"] = new PerlinNoise();
	m_textures["Cloud"]->load("Cloud");
	Logging::GRAPHICS->info("Done Loading Textures");
}

void TextureManager::loadTexture(string sFileName)
{
	Texture *texture = new TgaTexture();
	if (texture->load(sFileName.c_str())) 
	{
		int start_pos = sFileName.rfind("/")+1;
		int end_pos = sFileName.rfind(".");
		string sName = sFileName.substr(start_pos,end_pos-start_pos);
		texture->setName(sName);
		m_textures[sName]=texture;
	} 
	else 
	{
		Logging::GRAPHICS->error("Texture failed to load: " + sFileName);
	}
}

void TextureManager::bindTexture(string sName)
{
	m_textures[sName]->use();
}

void TextureManager::deleteTexture(string sName)
{
	if (m_textures.find(sName) != m_textures.end()) 
	{
		m_textures[sName]->remove();
		delete m_textures[sName];
		m_textures.erase(sName);
	}
}

void TextureManager::deleteAllTextures()
{
	m_textures.clear();
}

Texture *TextureManager::getTexture(string sName) 
{ 
	return m_textures[sName]; 
}

map<string,Texture *> *TextureManager::getTextures() 
{
	return &m_textures;
}