#ifndef WORLDMANAGER_H
#define WORLDMANAGER_H

#include <string>
#include "WorldChunks.h"
#include "ChunkManager.h"
#include "WorldTiles.h"
#include "TileManager.h"
#include "DirectLight.h"
#include "TimeManager.h"
#include "GameTime.h"
using namespace std;

class WorldManager
{
private:
	WorldChunks *m_worldChunks;
	WorldTiles *m_worldTiles;
	ChunkManager *m_chunks;
	TileManager *m_tiles;
	DirectLight *m_sun;

public:
	WorldManager();

	void tick(int nFps);
	void loadWorld(string sFilename);
	void generateNewWorld(ChunkManager *chunks, TileManager *tiles, PhysicsManager *physicsManager);
	void renderWorld(string sShader, Frustum *frustum = 0);
	void updateSunToGameTime(GameTime time);

	WorldChunks *getWorldChunks();
	DirectLight *getSun();
};

#endif