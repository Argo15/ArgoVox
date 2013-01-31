#ifndef SAVECHUNK_H
#define SAVECHUNK_H

#include <string>
#include "Actor.h"
#include "PhysicsShape.h"

class SaveChunk {
private:
	struct SaveActor
	{
		char name[100];
		char model[100];
		char material[100];

		float translation[3];
		float rotation[4];
		float scale[3];
	};
	struct SavePhysics
	{
		float translation[3];
		float rotation[4];
		float scale[3];
		PhysicsShapeType physicsType;
	};
	int numActors;
	SaveActor *actors;
	int numPhysics;
	SavePhysics *physics;
	int tileModes[10][10];  // 0=green, 1=red, 2=yellow

public:
	SaveChunk();

	void save(std::string filename);
};

#endif