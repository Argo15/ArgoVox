#ifndef WORLDSTATE_H
#define WORLDSTATE_H

#include <string>
#include "GameState.h"
#include "ChunkManager.h"
#include "TileManager.h"
#include "WorldRenderer.h"
#include "PhysicsManager.h"
#include "WorldManager.h"
#include "ShadowMapManager.h"

class WorldRenderer;
class ShadowMapManager;

class WorldState : public GameState
{
private:
	ChunkManager *m_chunkManager;
	TileManager *m_tileManager;
	WorldRenderer *m_renderer;
	PhysicsManager *m_physicsManager;
	WorldManager *m_worldManager;
	ShadowMapManager *m_shadowMapManager;

	bool m_bMouseHide;

public:
	WorldState(const char *sFilename = 0);
	~WorldState();

	virtual void resize(int nWidth, int nHeight);
	virtual void tick(int nFps);

	virtual bool mouseHidden();

	ChunkManager *getChunkManager();
	PhysicsManager *getPhysicsManager();
	WorldManager *getWorldManager();
	WorldRenderer *getRenderer();
	ShadowMapManager *getShadowMapManager();
	TileManager *getTileManager();
};
#endif