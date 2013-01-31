#ifndef	WORLDCHUNKS
#define WORLDCHUNKS

#include <vector>
#include "Chunk.h"
#include "ChunkManager.h"
#include "Frustum.h"
using namespace std;

class WorldChunks
{
private:
	int m_nWidth, m_nHeight;
	Chunk ***m_worldChunks;
	vector<Chunk *> *m_allChunks;

public:
	WorldChunks(int nWidth, int nHeight);

	void generateChunks(ChunkManager *chunks, PhysicsManager *physicsManager);
	vector<Chunk *> *getAllChunks();
	vector<Chunk *> *getVisibleChunks(Frustum *frustum);
	Chunk *getChunk(int x, int y);
	int getWidth();
	int getHeight();
};

#endif