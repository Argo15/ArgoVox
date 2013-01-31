#include "SceneManager.h"
#include "DrawFunc.h"
#include "ActorWidget.h"

SceneManager* SceneManager::m_pInstance = 0;

SceneManager::SceneManager() {
	nextActorId = 1;
	actors = new std::map<int, Actor *>();
	physicsShapes = new std::map<int, PhysicsShape *>();
	tileManager = new TileManager();
	chunkManager = new ChunkManager();
	tileManager->initialize();
	chunkManager->initialize();
	selectedActorId = -1;

	sceneTiles = new SceneTiles();

	displayScene = true;
	displayPhysics = false;
	displayTiles = false;

	editMode = 0;
}

SceneManager *SceneManager::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new SceneManager();
	}
	return m_pInstance;
}

int SceneManager::addActor(Actor *actor) {
	actors[0][nextActorId] = actor;
	setSelectedActor(nextActorId);
	return nextActorId++;
}

int SceneManager::addPhysicsShape(PhysicsShape *physicsShape) {
	int id = addActor(physicsShape);
	physicsShapes[0][id] = physicsShape;
	return id;
}

void SceneManager::removeActor(int actorId) {
	actors->erase(actorId);
}

void SceneManager::removeSelected() {
	removeActor(selectedActorId);
	setSelectedActor(-1);
}

void SceneManager::addChunk(Chunk *chunk) {
	Actor **chunkActors = chunk->getActors();
	int numActors = chunk->getNumActors();
	for (int i=0; i<numActors; i++) {
		Actor *newActor = new Actor();
		*newActor = *chunkActors[i];
		addActor(newActor);
	}

	PhysicsShape **chunkPhysics = chunk->getPhysics();
	int numPhysics = chunk->getNumPhysics();
	for (int i=0; i<numPhysics; i++) {
		PhysicsShape *newPhysics = new PhysicsShape(BOX);
		*newPhysics = *chunkPhysics[i];
		if (newPhysics->getShapeType() == BOX) {
			newPhysics->setModel(new string("Cube"));
		} else if (newPhysics->getShapeType() == SPHERE) {
			newPhysics->setModel(new string("Sphere"));
		} else if (newPhysics->getShapeType() == CYLINDER) {
			newPhysics->setModel(new string("Cylinder"));
		}
		newPhysics->setMaterial(new string("Physics"));
		addPhysicsShape(newPhysics);
	}

	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			sceneTiles->setTileMode(i,j,chunk->getTileMode(i,j));
		}
	}
}

void SceneManager::addChunk(string chunk) {
	addChunk(chunkManager->getChunk(chunk));
}

void SceneManager::addTile(string tile) {
	addChunk(tileManager->getTile(tile));
}

void SceneManager::clear() {
	actors->clear();
	setSelectedActor(-1);
}

void SceneManager::draw() {
	for (map<int,Actor*>::iterator it = actors->begin() ; it != actors->end(); it++ ) {
		if ((!isPhysicsObject((*it).first) && displayScene) || (isPhysicsObject((*it).first) && displayPhysics)) {
			Actor *actor = (*it).second;
			MatrixManager::getInstance()->pushMatrix4(MODELVIEW);
			MatrixManager::getInstance()->pushMatrix3(NORMAL);
				MatrixManager::getInstance()->putMatrix4(MODELVIEW, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
				MatrixManager::getInstance()->putMatrix3(NORMAL, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix3(NORMAL)));
				actor->drawActor("Basic");
			MatrixManager::getInstance()->popMatrix4(MODELVIEW);
			MatrixManager::getInstance()->popMatrix3(NORMAL);
		}
	}
	if (displayTiles) {
		sceneTiles->draw();
	}
}

void SceneManager::setSelectedActor(int id) {
	selectedActorId = id;
	ActorWidget::getInstance()->refresh();
}

Actor *SceneManager::getSelectedActor() {
	if (selectedActorId >= 0) {
		Actor *actor = actors[0][selectedActorId];
		return actor;
	}
	return 0;
}

void SceneManager::drawTransformers() {
	if (selectedActorId >= 0) {
		Actor *actor = actors[0][selectedActorId];
		glPushMatrix();
		transformNoShaders();
		glTranslatef(actor->getTranslate()[0], actor->getTranslate()[1], actor->getTranslate()[2]);
		glDepthFunc(GL_ALWAYS);
		Transformer::draw(false);
		glDepthFunc(GL_LEQUAL);
		glPopMatrix();
	}
}