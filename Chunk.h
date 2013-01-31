#ifndef CHUNK_H
#define CHUNK_H

#include <string>
#include <iostream>
#include <fstream>
#include "Entity.h"
#include "Actor.h"
#include "PhysicsShape.h"
#include "PhysicsManager.h"
using namespace std;

class Chunk : public Entity, public Transformable 
{
protected:
	struct SaveActor
	{
		char sName[100];
		char sModel[100];
		char sMaterial[100];

		float nTranslation[3];
		float nRotation[4];
		float nScale[3];
	};
	struct SavePhysics
	{
		float nTranslation[3];
		float nRotation[4];
		float nScale[3];
		PhysicsShapeType m_physicsType;
	};
	int m_nNumActors;
	Actor **m_actors;
	int m_nNumPhysics;
	PhysicsShape **m_physics;
	int m_nOrientation;
	float m_nRadius;
	int m_nTileModes[10][10];  // 0=green, 1=red, 2=yellow
	virtual void drawExtra(string shader);

public:
	Chunk();

	bool loadChunk(string sFilesName);
	void drawChunk(string sShader);
	void setOrientation(int nOrientation);
	int getOrientation();
	float getRadius();
	void addPhysicsToDynamicWorld(PhysicsManager *physicsManager);
	int getTileMode(int nPosX, int nPosY);
	Actor **getActors();
	int getNumActors();
	PhysicsShape **getPhysics();
	int getNumPhysics();
};

#endif