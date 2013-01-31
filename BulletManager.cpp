#include "BulletManager.h"
#include "WorldState.h"
#include "Profiler.h"
#include "GameState.h"
#include "InputManager.h"

BulletManager::BulletManager()
{
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0,-10,0));
	m_playerBody = 0;
}

void BulletManager::tick(int nFps)
{
	if (m_playerBody == 0)
	{
		createPlayerBody();
	}
}

void BulletManager::addPhysicsShape(PhysicsShape *shape)
{
	btCollisionShape* bodyShape = shape->getCollisionShape();
	if (bodyShape == 0) 
	{
		return;
	}
	btTransform bodyTransform;
	bodyTransform.setIdentity();
	bodyTransform.setFromOpenGLMatrix(&shape->matrix[0][0]);
	btScalar mass(0.);
	btVector3 localInertia(0,0,0);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,bodyShape,localInertia);
	btRigidBody *body = new btRigidBody(rbInfo);
	m_dynamicsWorld->addRigidBody(body);
}

void BulletManager::clearDynamicsWorld()
{
	delete m_dynamicsWorld;
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
	m_dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,overlappingPairCache,solver,collisionConfiguration);
	m_playerBody = 0;
}

void BulletManager::createPlayerBody()
{
	btCollisionShape* bodyShape = new btCapsuleShape(.3f,2.0f);
	WorldState *worldState = (WorldState *) GameState::GAMESTATE;
	Camera *camera = worldState->getPhysicsManager()->getWorldCameras()->getPlayerCamera();
	btTransform bodyTransform;
	bodyTransform.setIdentity();
	float *translate = camera->getEye();
	bodyTransform.setOrigin(btVector3(translate[0],translate[1]-0.79f,translate[2]));
	btScalar mass(10.);
	btVector3 localInertia(0,0,0);
	bodyShape->calculateLocalInertia(mass,localInertia);
	btDefaultMotionState* myMotionState = new btDefaultMotionState(bodyTransform);
	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,bodyShape,localInertia);
	m_playerBody = new btRigidBody(rbInfo);
	m_playerBody->setAngularFactor(0);
	m_dynamicsWorld->addRigidBody(m_playerBody);
}

void BulletManager::updateDynamicsWorld(float nSpeed)
{
	Profiler::getInstance()->startProfile("Update Dynamics World");
	// move camera
	WorldState *worldState = (WorldState *) GameState::GAMESTATE;
	WorldCamera *camera = worldState->getPhysicsManager()->getWorldCameras()->getPlayerCamera();
	float nOldEye[3];
	nOldEye[0] = camera->getEyeX();
	nOldEye[1] = camera->getEyeY();
	nOldEye[2] = camera->getEyeZ();
	if (InputManager::getInstance()->isKeyDown('w'))
	{
		camera->moveForward(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('s'))
	{
		camera->moveBackward(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('a'))
	{
		camera->moveLeft(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('d'))
	{
		camera->moveRight(nSpeed*0.1f);
	}
	float *nNewEye = camera->getEye();
	m_playerBody->activate(true);
	m_playerBody->setLinearVelocity(100.0f*btVector3(nNewEye[0]-nOldEye[0],-0.04f,nNewEye[2]-nOldEye[2]));

	// check physics
	m_dynamicsWorld->stepSimulation(1.f/60.f,10);

	// update camera
	btVector3 camPos = m_playerBody->getWorldTransform().getOrigin();
	camera->setPosition(camPos[0],camPos[1]+0.8,camPos[2]);
	Profiler::getInstance()->endProfile();
}