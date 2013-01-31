#include "WorldState.h"
#include "Matrix.h"
#include "TimeManager.h"
#include "InputManager.h"

WorldState::WorldState(const char *sFilename)
{ 
	m_physicsManager = new PhysicsManager();
	m_chunkManager = new ChunkManager();
	m_chunkManager->initialize();
	m_tileManager = new TileManager();
	m_tileManager->initialize();
	m_renderer = new WorldRenderer();
	m_renderer->init();
	glutSetCursor(GLUT_CURSOR_NONE);

	m_worldManager = new WorldManager();
	if (sFilename == 0)
	{
		m_worldManager->generateNewWorld(m_chunkManager, m_tileManager, m_physicsManager);
	}
	else
	{
		m_worldManager->loadWorld(string(sFilename));
	}

	m_shadowMapManager = new ShadowMapManager();

	m_bMouseHide = true;
}

WorldState::~WorldState()
{

}

void WorldState::resize(int nWidth, int nHeight)
{
	m_renderer->resize(nWidth, nHeight);
}

void WorldState::tick(int nFps)
{
	if (InputManager::getInstance()->isKeyDownOnce((int)'t'))
	{
		m_bMouseHide = !m_bMouseHide;
		if (m_bMouseHide)
		{
			glutSetCursor(GLUT_CURSOR_NONE);
		}
		else
		{
			glutSetCursor(GLUT_CURSOR_RIGHT_ARROW);
		}
	}

	m_physicsManager->tick(nFps);
	m_worldManager->tick(nFps);
	m_shadowMapManager->tick(nFps);
	m_renderer->render();
}

bool WorldState::mouseHidden()
{
	return m_bMouseHide;
}

ChunkManager *WorldState::getChunkManager()
{
	return m_chunkManager;
}
	
PhysicsManager *WorldState::getPhysicsManager()
{
	return m_physicsManager;
}
	
WorldManager *WorldState::getWorldManager()
{
	return m_worldManager;
}
	
WorldRenderer *WorldState::getRenderer()
{
	return m_renderer;
}
	
ShadowMapManager *WorldState::getShadowMapManager()
{
	return m_shadowMapManager;
}
	
TileManager *WorldState::getTileManager()
{
	return m_tileManager;
}