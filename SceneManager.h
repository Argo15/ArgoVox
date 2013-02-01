#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include <vector>
#include "Actor.h"
#include "Tile.h"
#include "TileManager.h"
#include "ChunkManager.h"
#include "Logger.h"
#include "Transformer.h"
#include "SceneTiles.h"
#include "PhysicsShape.h"

class Transformer;

class SceneManager {
private:
	static SceneManager *m_pInstance;
	std::map<int, Actor *> *actors;				// ActorId -> Actor
	std::map<int, PhysicsShape *> *physicsShapes;	// ActorId -> Actor
	int nextActorId;
	TileManager *tileManager;
	ChunkManager *chunkManager;
	int selectedActorId;
	SceneManager();

	SceneTiles *sceneTiles;
	int editMode;

	bool displayScene, displayPhysics, displayTiles;

	QGLWidget *glWidget;

public:
	static SceneManager *getInstance();
	int addActor(Actor *actor);			// returns an actorId
	int addPhysicsShape(PhysicsShape *physicsShape);	// returns an actorId
	void removeActor(int actorId);
	void removeSelected();
	void addChunk(Chunk *chunk);
	void addChunk(string chunk);
	void addTile(string tile);
	void clear();
	void draw(string shader);
	void drawTransformers();
	void setSelectedActor(int id);
	Actor *getSelectedActor();
	std::map<int, Actor *> *getActors() {return actors;}
	std::map<int, PhysicsShape *> *getPhysics() {return physicsShapes;}

	TileManager *getTileManager() {return tileManager;}
	ChunkManager *getChunkManager() {return chunkManager;}

	void setGLWidget(QGLWidget *widget) {glWidget = widget;}
	QGLWidget *getGLWidget() {return glWidget;}

	void setDisplayScene(bool value) {displayScene = value;}
	void setDisplayPhysics(bool value) {displayPhysics = value;}
	void setDisplayTiles(bool value) {displayTiles = value;}
	bool getDisplayScene() {return displayScene;}
	bool getDisplayPhysics() {return displayPhysics;}
	bool getDisplayTiles() {return displayTiles;}

	void setEditMode(int value) {editMode = value;}
	int getEditMode() {return editMode;}

	SceneTiles *getSceneTiles() {return sceneTiles;}

	bool isPhysicsObject(int id) {return physicsShapes->find(id) != physicsShapes->end();}
};

#endif