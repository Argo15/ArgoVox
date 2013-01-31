#include "WorldCamera.h"
#include "WorldState.h"
#include "GameState.h"

WorldCamera::WorldCamera() : Camera() 
{
	m_nHAngle=0;
    m_nVAngle=0;
	m_nEyePos[0]=0.0; m_nEyePos[1]=1.8f; m_nEyePos[2]=0.0; 
	m_nSpeedFactor = 0.3f;
	m_nRotSpeed = 0.3f;
}

void WorldCamera::move(float nSpeed)
{
	WorldState *worldState = (WorldState *) GameState::GAMESTATE;
	worldState->getPhysicsManager()->getBulletManager()->updateDynamicsWorld(nSpeed);

	mouseRotate();
}

void WorldCamera::moveForward(float nSpeed)
{
	m_nEyePos[0]-=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nEyePos[2]+=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[0]-=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[2]+=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
}

void WorldCamera::moveBackward(float nSpeed)
{
	m_nEyePos[0]+=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nEyePos[2]-=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[0]+=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[2]-=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
}

void WorldCamera::moveLeft(float nSpeed)
{
	m_nEyePos[2]+=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nEyePos[0]+=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[2]+=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[0]+=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
}

void WorldCamera::moveRight(float nSpeed)
{
	m_nEyePos[2]-=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nEyePos[0]-=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[2]-=cos(m_nHAngle)*nSpeed*m_nSpeedFactor;
	m_nLookAt[0]-=sin(m_nHAngle)*nSpeed*m_nSpeedFactor;
}

void WorldCamera::moveUp(float nSpeed)
{

}
	
void WorldCamera::moveDown(float nSpeed) 
{

}