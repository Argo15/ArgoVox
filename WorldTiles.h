#ifndef WORLDTILES_H
#define WORLDTILES_H

#include <vector>
#include "Tile.h"
#include "TileManager.h"
#include "WorldChunks.h"
#include "Frustum.h"
using namespace std;

enum TileMode
{
	GREEN,
	RED,
	YELLOW
};

class WorldTiles
{
private:
	struct TileData
	{
		TileMode tileMode; 
		vector<Tile *> *tiles;
	};

	int m_nWidth, m_nHeight;
	TileData ***m_worldTiles;
	vector<Tile *> *m_allTiles;

public:
	WorldTiles(int nWidth, int nHeight);

	void init();
	void initializeFromChunks(WorldChunks *chunks, TileManager *manager, PhysicsManager *physicsManager);
	bool addTile(int nPosX, int nPosY, Tile *tile, PhysicsManager *physicsManager);
	vector<Tile *> *getAllTiles();
	vector<Tile *> *getVisibleTiles(Frustum *frustum);
	int getWidth();
	int getHeight();
};

#endif