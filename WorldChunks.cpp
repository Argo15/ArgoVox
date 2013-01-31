#include "WorldChunks.h"
#include "WorldState.h"
#include <time.h>

WorldChunks::WorldChunks(int nWidth, int nHeight)
{
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	m_worldChunks = new Chunk**[nWidth];
	for (int i=0; i<nWidth; i++)
	{
		m_worldChunks[i] = new Chunk*[nHeight];
		for (int j=0; j<nHeight; j++)
		{
			m_worldChunks[i][j] = new Chunk();
		}
	}
}

void WorldChunks::generateChunks(ChunkManager *chunks, PhysicsManager *physicsManager)
{
	string sChunkNames[12];
	sChunkNames[0] = "Flat Chunk";
	sChunkNames[1] = "River Chunk";
	sChunkNames[2] = "River Corner Chunk";
	sChunkNames[3] = "Wall Chunk";
	sChunkNames[4] = "Wall Corner Chunk";
	sChunkNames[5] = "Bridge River Chunk";
	sChunkNames[6] = "Bridge River Corner Chunk";
	sChunkNames[7] = "Water Plane";
	sChunkNames[8] = "Beach Chunk";
	sChunkNames[9] = "Beach River Chunk";
	sChunkNames[10] = "Invisible Wall";

	srand ( time(NULL) );

	int nLeftCount = 0;
	int nRightCount = 0;
	do
	{
		m_allChunks = new vector<Chunk *>();

		nLeftCount = 0;
		nRightCount = 0;
		int nBridgeCount = 0;

		// build a flat world
		for (int i=0; i<m_nWidth; i++)
		{
			for (int j=0; j<m_nHeight-1; j++)
			{
				*m_worldChunks[i][j] = *(chunks->getChunk(sChunkNames[0]));
				m_worldChunks[i][j]->setTranslate(i*10.0f,0,j*10.0f);
			}
		}
		for (int i=0; i< m_nWidth; i++)
		{
			*m_worldChunks[i][m_nHeight-1] = *(chunks->getChunk(sChunkNames[8]));
			m_worldChunks[i][m_nHeight-1]->setTranslate(i*10.0f,0,(m_nHeight-1)*10.0f);
		}

		// pick a place to start a river
		glm::ivec2 vStartPoint;
		do
		{
			vStartPoint = glm::ivec2(rand()%(m_nWidth-4)+2,0);
		} 
		while (abs(vStartPoint.x-m_nWidth/2) < 2);

		*m_worldChunks[vStartPoint.x][vStartPoint.y] = *(chunks->getChunk(sChunkNames[1]));
		m_worldChunks[vStartPoint.x][vStartPoint.y]->setTranslate(vStartPoint.x*10.0f,0,vStartPoint.y*10.0f);
		m_worldChunks[vStartPoint.x][vStartPoint.y]->setOrientation(1);
		Chunk *waterChunk = new Chunk();
		*waterChunk = *(chunks->getChunk(sChunkNames[7]));
		waterChunk->setTranslate(vStartPoint.x*10.0f,0,vStartPoint.y*10.0f);
		m_allChunks->push_back(waterChunk);

		// carve out a river
		glm::ivec2 vCurrentPoint = vStartPoint;
		int nDirection = 0;
		int nLastDirection = 0;
		int nLookingDir = 0;
		int nOrientation = 0;
		do
		{
			// get current location
			if (nDirection == 0)
			{
				vCurrentPoint.y = vCurrentPoint.y + 1;
				nOrientation = 1;
			}
			if (nDirection == 1)
			{
				vCurrentPoint.x = vCurrentPoint.x + 1;
				nOrientation = 0;
			}
			if (nDirection == 2)
			{
				vCurrentPoint.x = vCurrentPoint.x - 1;
				nOrientation = 0;
			}

			// determine if we need to add a bridge
			int nBridgeOffset = 0;
			if ((vCurrentPoint.y > 1 && nBridgeCount < 1) || (vCurrentPoint.y > 4 && nBridgeCount < 2) || (vCurrentPoint.y > 7 && nBridgeCount < 3))
			{
				nBridgeOffset = 4;
				nBridgeCount++;
			}

			// set river chunk, and move a nDirection
			*m_worldChunks[vCurrentPoint.x][vCurrentPoint.y] = *(chunks->getChunk(sChunkNames[1+nBridgeOffset]));
			m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setTranslate(vCurrentPoint.x*10.0f,0,vCurrentPoint.y*10.0f);
			m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setOrientation(nOrientation);
			nLastDirection = nDirection;
			if (nDirection == 0 && rand()%2 == 0)
			{
				nDirection = rand()%2+1;
			}
			else if ((nDirection == 1 ||  nDirection == 2) && rand()%2 == 0)
			{
				nDirection = 0;
			}
			if (vCurrentPoint.x >= m_nWidth-2 && nDirection == 1)
			{
				nDirection = 0;
			}
			if (vCurrentPoint.x <= 1 && nDirection == 2)
			{
				nDirection = 0;
			}
			if (vCurrentPoint.y <= 1 || vCurrentPoint.y >= m_nHeight-2)
			{
				nDirection = 0;
			}
			if (nDirection == 1 || nDirection == 2)
			{
				if (((*m_worldChunks[vCurrentPoint.x+3-2*nDirection][vCurrentPoint.y-1]->getName() == sChunkNames[1]) ||
					(*m_worldChunks[vCurrentPoint.x+3-2*nDirection][vCurrentPoint.y-1]->getName() == sChunkNames[2]) ||
					(*m_worldChunks[vCurrentPoint.x+3-2*nDirection][vCurrentPoint.y-1]->getName() == sChunkNames[5]) ||
					(*m_worldChunks[vCurrentPoint.x+3-2*nDirection][vCurrentPoint.y-1]->getName() == sChunkNames[6])))
				{
					nDirection = 0;
				}
			}

			// Check if a turn was made, and replace chunk if needed
			if (nDirection == 0 && nLastDirection != 0 )
			{
				*m_worldChunks[vCurrentPoint.x][vCurrentPoint.y] = *(chunks->getChunk(sChunkNames[2+nBridgeOffset]));
				m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setTranslate(vCurrentPoint.x*10.0f,0,vCurrentPoint.y*10.0f);
				m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setOrientation(nLastDirection + 2);
			}
			else if ( nDirection != 0 && nLastDirection == 0 )
			{
				*m_worldChunks[vCurrentPoint.x][vCurrentPoint.y] = *(chunks->getChunk(sChunkNames[2+nBridgeOffset]));
				m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setTranslate(vCurrentPoint.x*10.0f,0,vCurrentPoint.y*10.0f);
				m_worldChunks[vCurrentPoint.x][vCurrentPoint.y]->setOrientation(nDirection);
			}

			Chunk *waterChunk = new Chunk();
			*waterChunk = *(chunks->getChunk(sChunkNames[7]));
			waterChunk->setTranslate(vCurrentPoint.x*10.0f,0,vCurrentPoint.y*10.0f);
			m_allChunks->push_back(waterChunk);

		}
		while (vCurrentPoint.y < m_nHeight-2 && vCurrentPoint.x < m_nWidth-1 && vCurrentPoint.x > 0);

		*m_worldChunks[vCurrentPoint.x][vCurrentPoint.y+1] = *(chunks->getChunk(sChunkNames[9]));
		m_worldChunks[vCurrentPoint.x][vCurrentPoint.y+1]->setTranslate(vCurrentPoint.x*10.0f,0,(vCurrentPoint.y+1)*10.0f);
		m_worldChunks[vCurrentPoint.x][vCurrentPoint.y+1]->setOrientation(nDirection);
		for (int j=0; j<m_nHeight; j++)
		{
			bool bLeft = true;
			for (int i=0; i<m_nWidth; i++)
			{
				if (*m_worldChunks[i][j]->getName() == sChunkNames[0])
				{
					if (bLeft)
					{
						nLeftCount++;
					}
					else
					{
						nRightCount++;
					}
				}
				if (((*m_worldChunks[i][j]->getName() == sChunkNames[1]) ||
					(*m_worldChunks[i][j]->getName() == sChunkNames[2]) ||
					(*m_worldChunks[i][j]->getName() == sChunkNames[5]) ||
					(*m_worldChunks[i][j]->getName() == sChunkNames[6])))
				{
					bLeft = false;
				}
			}
		}
	}
	while (abs(nLeftCount-nRightCount) > 5);

	// add the edges
	for (int i=0; i< m_nWidth; i++)
	{
		Chunk *edgeChunk = new Chunk();
		*edgeChunk = *(chunks->getChunk(sChunkNames[3]));
		edgeChunk->setTranslate(i*10.0f,0,-10.0f);
		edgeChunk->setOrientation(2);
		m_allChunks->push_back(edgeChunk);
	}
	for (int i=0; i<m_nHeight; i++)
	{
		Chunk *edgeChunk = new Chunk();
		*edgeChunk = *(chunks->getChunk(sChunkNames[3]));
		edgeChunk->setTranslate(-10.0f,0,i*10.0f);
		edgeChunk->setOrientation(3);
		m_allChunks->push_back(edgeChunk);
		edgeChunk = new Chunk();
		*edgeChunk = *(chunks->getChunk(sChunkNames[3]));
		edgeChunk->setTranslate(m_nWidth*10.0f,0,i*10.0f);
		edgeChunk->setOrientation(1);
		m_allChunks->push_back(edgeChunk);
	}

	// add ocean
	for (int i=-3; i<m_nWidth+3; i++)
	{
		for (int j=0; j<3; j++)
		{
			Chunk *waterChunk = new Chunk();
			*waterChunk = *(chunks->getChunk(sChunkNames[7]));
			waterChunk->setTranslate(i*10.0f,0,(m_nHeight+j)*10.0f);
			m_allChunks->push_back(waterChunk);
		}
	}

	// add the corners
	Chunk *cornerChunk = new Chunk();
	*cornerChunk = *(chunks->getChunk(sChunkNames[4]));
	cornerChunk->setTranslate(-10.0f,0,-10.0f);
	m_allChunks->push_back(cornerChunk);
	cornerChunk = new Chunk();
	*cornerChunk = *(chunks->getChunk(sChunkNames[4]));
	cornerChunk->setTranslate(m_nWidth*10.0f,0,-10.0f);
	m_allChunks->push_back(cornerChunk);
	cornerChunk = new Chunk();

	// add world chunks to array
	for (int i=0; i<m_nWidth; i++)
	{
		for (int j=0; j<m_nHeight; j++)
		{
			m_allChunks->push_back(m_worldChunks[i][j]);
		}
	}

	// add physics to world
	for (vector<Chunk *>::iterator i = m_allChunks->begin(); i != m_allChunks->end(); i++)
	{
		(*i)->addPhysicsToDynamicWorld(physicsManager);
	}
}

vector<Chunk *> *WorldChunks::getAllChunks()
{
	return m_allChunks;
}

vector<Chunk *> *WorldChunks::getVisibleChunks(Frustum *frustum)
{
	vector<Chunk *> *chunks = new vector<Chunk *>();
	for (vector<Chunk *>::iterator i = m_allChunks->begin(); i != m_allChunks->end(); i++)
	{
		if (frustum->isInFrustum((*i)->getTranslateV(),(*i)->getRadius()))
		{
			chunks->push_back((*i));
		}
	}
	return chunks;
}

Chunk *WorldChunks::getChunk(int x, int y)
{
	return m_worldChunks[x][y];
}

int WorldChunks::getWidth()
{
	return m_nWidth;
}

int WorldChunks::getHeight()
{
	return m_nHeight;
}