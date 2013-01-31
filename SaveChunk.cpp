#include "SaveChunk.h"
#include "SceneManager.h"

SaveChunk::SaveChunk()
{
	
}

void SaveChunk::save(std::string filename)
{
	std::map<int, PhysicsShape *> *hmPhysics = SceneManager::getInstance()->getPhysics();
	numPhysics = hmPhysics->size();

	// Actor section
	std::map<int, Actor *> *hmActors = SceneManager::getInstance()->getActors();
	numActors = hmActors->size()-numPhysics;
	actors = new SaveActor[numActors];
	int i=0;
	for (std::map<int, Actor *>::iterator it = hmActors->begin(); it != hmActors->end(); it++)
	{
		int actorId = (*it).first;
		if (!SceneManager::getInstance()->isPhysicsObject(actorId))
		{
			Actor *actor = (*it).second;
			char idBuff[10];
			itoa(actorId, idBuff, 10);
			std::string sActorName("Actor " + std::string(idBuff));
			strcpy(actors[i].name, sActorName.c_str());
			strcpy(actors[i].material, actor->getMaterial()->c_str());
			strcpy(actors[i].model, actor->getModel()->c_str());
			for (int j=0; j<3; j++) {
				actors[i].rotation[j] = actor->getRotate()[j];
				actors[i].scale[j] = actor->getScale()[j];
				actors[i].translation[j] = actor->getTranslate()[j];
			}
			actors[i].rotation[3] = actor->getRotate()[3];
			i++;
		}
	}

	// Physics section
	physics = new SavePhysics[numPhysics];
	i=0;
	for (std::map<int, PhysicsShape *>::iterator it = hmPhysics->begin(); it != hmPhysics->end(); it++)
	{
		PhysicsShape *physicsShape = (*it).second;
		for (int j=0; j<3; j++) {
			physics[i].rotation[j] = physicsShape->getRotate()[j];
			physics[i].scale[j] = physicsShape->getScale()[j];
			physics[i].translation[j] = physicsShape->getTranslate()[j];
		}
		physics[i].rotation[3] = physicsShape->getRotate()[3];
		physics[i].physicsType = physicsShape->getShapeType();
		i++;
	}

	// Tile section
	SceneTiles *tiles = SceneManager::getInstance()->getSceneTiles();
	for (int x=0; x<10; x++) {
		for (int y=0; y<10; y++) {
			tileModes[x][y] = tiles->getTileMode(x,y);
		}
	}

	ofstream file(filename.c_str(), ios::out|ios::binary|ios::ate);
	if (file.is_open())
	{ 
		file.write((char*)&numActors,sizeof(numActors));
		file.write((char*)actors,sizeof(SaveActor)*numActors);
		file.write((char*)&numPhysics,sizeof(numPhysics));
		file.write((char*)physics,sizeof(SavePhysics)*numPhysics);
		file.write((char*)tileModes,sizeof(tileModes));

		file.close();
	}
}