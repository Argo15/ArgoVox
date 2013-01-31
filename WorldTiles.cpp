#include "WorldTiles.h"
#include "Tree.h"

WorldTiles::WorldTiles(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
}

void WorldTiles::init()
{
	m_worldTiles = new TileData**[m_nWidth];
	for (int i=0; i<m_nWidth; i++)
	{
		m_worldTiles[i] = new TileData*[m_nHeight];
		for (int j=0; j<m_nHeight; j++)
		{
			m_worldTiles[i][j] = new TileData();
			m_worldTiles[i][j]->tileMode = GREEN;
			m_worldTiles[i][j]->tiles = new vector<Tile *>();
		}
	}
	m_allTiles = new vector<Tile *>();
}

void WorldTiles::initializeFromChunks(WorldChunks *chunks, TileManager *manager, PhysicsManager *physicsManager)
{
	init();
	for (int i=0; i<(m_nWidth/10); i++)
	{
		for (int j=0; j<(m_nHeight/10); j++)
		{
			Chunk *chunk = chunks->getChunk(i,j);
			for (int x=0; x<10; x++)
			{
				for (int y=0; y<10; y++)
				{
					m_worldTiles[i*10+x][j*10+y]->tileMode = (TileMode) chunk->getTileMode(x,y);
				}
			}
		}
	}
	for (int i=0; i<m_nWidth; i++)
	{
		for (int j=0; j<m_nHeight; j++)
		{
			Tile *blockTile = new Tree();
			*blockTile = *(Tree*)manager->getTile("Tree");
			blockTile->translate(i-5.0f, 0, j-5.0f);
			if (rand() % 50 == 1)
			{
				addTile(i,j,blockTile,physicsManager);
			}
		}
	}
}

vector<Tile *> *WorldTiles::getAllTiles()
{
	return m_allTiles;
}

vector<Tile *> *WorldTiles::getVisibleTiles(Frustum *frustum)
{
	vector<Tile *> *tiles = new vector<Tile *>();
	for (vector<Tile *>::iterator i = m_allTiles->begin(); i != m_allTiles->end(); i++)
	{
		if (frustum->isInFrustum((*i)->getTranslateV(),(*i)->getRadius()))
		{
			tiles->push_back((*i));
		}
	}
	return tiles;
}

bool WorldTiles::addTile(int nPosX, int nPosY, Tile *tile, PhysicsManager *physicsManager)
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			int nWorldX = nPosX+i-5;
			int nWorldY = nPosY+j-5;
			int nTileTileMode = (TileMode) tile->getTileMode(i,j); // tile's local tile modes
			if (nWorldX >=0 && nWorldX < 100 && nWorldY >= 0 && nWorldY < 100) // inside the tileable zone
			{
				int worldTileMode = (TileMode) m_worldTiles[nWorldX][nWorldY]->tileMode;
				// red and yellow can't hit red or yellow
				if ((nTileTileMode == RED || nTileTileMode == YELLOW) && (worldTileMode >= RED || worldTileMode == YELLOW))	
				{
					return false;
				}
			}
			else	// outside tileable zone
			{
				if (nTileTileMode != 0) // only green allowed here
				{
					return false;
				}
			}
		}
	}

	// success, update the world tile modes
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			int nWorldX = nPosX+i-5;
			int nWorldY = nPosY+j-5;
			int nTileTileMode = tile->getTileMode(i,j); // tile's local tile modes
			if (nWorldX >=0 && nWorldX < 100 && nWorldY >= 0 && nWorldY < 100) // inside the tileable zone
			{ 
				if (m_worldTiles[nWorldX][nWorldY]->tileMode == 0)
				{
					m_worldTiles[nWorldX][nWorldY]->tileMode = (TileMode) nTileTileMode; // green -> yellow/red
				}
				// red stays red
				// yellow can only update to red
				if (m_worldTiles[nWorldX][nWorldY]->tileMode == 2)
				{
					if (nTileTileMode == 1)
					{
						m_worldTiles[nWorldX][nWorldY]->tileMode = (TileMode) nTileTileMode; // yellow -> red
					}
				}
			}
		}
	}
	m_worldTiles[nPosX][nPosY]->tiles->push_back(tile);
	m_allTiles->push_back(tile);
	tile->addPhysicsToDynamicWorld(physicsManager);
	return true;
}

int WorldTiles::getWidth()
{
	return m_nWidth;
}

int WorldTiles::getHeight()
{
	return m_nHeight;
}