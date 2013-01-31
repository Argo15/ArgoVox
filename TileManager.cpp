#include "TileManager.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include "dirent.h"
#include "Logger.h"

TileManager::TileManager()
{
	
}

void TileManager::initialize()
{
	Logging::GRAPHICS->info("Loading Tiles");
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("./Data/Tiles") )
	{
		while(entry = readdir(pDIR))
		{
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				loadTile(string("./Data/Tiles/") + entry->d_name);
			}
		}
		closedir(pDIR);
	}
	Logging::GRAPHICS->info("Done Loading Tiles");
}

void TileManager::loadTile(string sFileName)
{
	Tile *tile = new Tile();
	if (tile->loadTile(sFileName)) 
	{
		int start_pos = sFileName.rfind("/")+1;
		int end_pos = sFileName.rfind(".");
		string sName = sFileName.substr(start_pos,end_pos-start_pos);
		tile->setName(sName);
		m_tiles[sName]=tile;
	} 
	else 
	{
		Logging::GRAPHICS->error("Tile failed to load: " + sFileName);
	}
}

void TileManager::drawTile(string sName, string sShader)
{
	m_tiles[sName]->drawTile(sShader);
}

void TileManager::deleteTile(string sName)
{
	if (m_tiles.find(sName) == m_tiles.end())
	{
		delete m_tiles[sName];
		m_tiles.erase(sName);
	}
}

void TileManager::deleteAllTiles()
{
	m_tiles.clear();
}

Tile *TileManager::getTile(string sName) 
{ 
	return m_tiles[sName]; 
}

map<string,Tile *> *TileManager::getTiles() 
{
	return &m_tiles;
}