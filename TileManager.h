#ifndef TILEMANAGER_H
#define TILEMANAGER_H

#include <string>
#include <map>
#include "Tile.h"
using namespace std;

class TileManager 
{
private:
	map<string,Tile *> m_tiles;

public:
	TileManager();

    void initialize();
	void loadTile(string sFileName);
	void drawTile(string sName, string sShader);
	void deleteTile(string sName);
	void deleteAllTiles();

	Tile *getTile(string sName);
	map<string,Tile *> *getTiles();
};

#endif