#include "WorldManager.h"
#include "WorldState.h"
#include "Camera.h"
#include "GameState.h"
#include "InputManager.h"

static Vector3 V_SUN_QUATERNION(2.0f,0.0f,-1.0f);

WorldManager::WorldManager()
{
	m_worldChunks = new WorldChunks(10,10);
	m_worldTiles = new WorldTiles(100,100);
	m_sun = new DirectLight();
}

void WorldManager::tick(int nFps)
{
	updateSunToGameTime(TimeManager::getInstance()->getGameTime());
	if(InputManager::getInstance()->isKeyDownOnce('r'))
	{
		WorldState *worldState = (WorldState *) GameState::GAMESTATE;
		worldState->getPhysicsManager()->getBulletManager()->clearDynamicsWorld();
		m_worldChunks->generateChunks(m_chunks, worldState->getPhysicsManager());
		m_worldTiles->initializeFromChunks(m_worldChunks, m_tiles, worldState->getPhysicsManager());
	}
}

void WorldManager::loadWorld(std::string sFilename)
{
	
}

void WorldManager::generateNewWorld(ChunkManager *chunks, TileManager *tiles, PhysicsManager *physicsManager)
{
	m_worldChunks->generateChunks(chunks, physicsManager);
	m_worldTiles->initializeFromChunks(m_worldChunks, tiles, physicsManager);
	this->m_chunks = chunks;
	this->m_tiles = tiles;
}

void WorldManager::renderWorld(string sShader, Frustum *frustum)
{
	WorldState *worldState = (WorldState *) GameState::GAMESTATE;
	if (frustum == 0)
	{
		frustum = worldState->getRenderer()->getFrustum();
	}
	vector<Chunk *> *visChunks = m_worldChunks->getVisibleChunks(frustum);
	for (vector<Chunk *>::iterator i = visChunks->begin(); i != visChunks->end(); i++)
	{
		(*i)->drawChunk(sShader);
	}
	vector<Tile *> *visTiles = m_worldTiles->getVisibleTiles(frustum);
	for (vector<Tile *>::iterator i = visTiles->begin(); i != visTiles->end(); i++)
	{
		(*i)->drawTile(sShader);
	}
}

void WorldManager::updateSunToGameTime(GameTime time)
{
	float angle = (time.getHours()/24.0f + time.getMinutes()/60.0f/24.0f + time.getSeconds()/60.0f/60.0f/24.0f) - 0.5f;
	Quaternion quat;
	quat.createQuaternion(angle*(2*3.1415), V_SUN_QUATERNION);
	m_sun->setRotate(quat);
}

WorldChunks *WorldManager::getWorldChunks()
{
	return m_worldChunks;
}
	
DirectLight *WorldManager::getSun()
{
	return m_sun;
}