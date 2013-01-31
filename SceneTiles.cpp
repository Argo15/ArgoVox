#include "SceneTiles.h"
#include "Unproject.h"
#include "InputManager.h"

SceneTiles::SceneTiles() {
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			tileModes[i][j] = GREEN;
			updatable[i][j] = true;
		}
	}
	tileManager = new TileManager();
	tileManager->initialize();
}

void SceneTiles::updateTiles()
{
	int mouseX = InputManager::getInstance()->getMouseX();
	int mouseY = InputManager::getInstance()->getMouseY();
	Vector3 rayStart = getUnproject(0.0, mouseX, mouseY);
	Vector3 rayEnd = getUnproject(1.0, mouseX, mouseY);
	Vector3 rayDir = rayEnd - rayStart;
	rayDir = rayDir.normalize();
	float t = (-1.0*rayStart[1])/rayDir[1];
	Vector3 intersect = rayStart + rayDir * t;
	selectedX = (int)(intersect[0]+5);
	selectedZ = (int)(intersect[2]+5);
	if (selectedX >= 0 && selectedX < 10 && selectedZ >= 0 && selectedZ < 10)
	{
		if (updatable[selectedX][selectedZ]) {
			tileModes[selectedX][selectedZ] = (tileModes[selectedX][selectedZ] + 1) % 3;
			updatable[selectedX][selectedZ] = false;
		}
	}
}

void SceneTiles::clearUpdatables() {
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			updatable[i][j] = true;
		}
	}
}

void SceneTiles::draw()
{
	Transformable transform;
	for (int i=0; i<10; i++) {
		for (int j=0; j<10; j++) {
			transform.setTranslate(i-5,0,j-5);
			MatrixManager::getInstance()->pushMatrix4(MODELVIEW);
			MatrixManager::getInstance()->pushMatrix3(NORMAL);
				MatrixManager::getInstance()->putMatrix4(MODELVIEW, transform.transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
				if (tileModes[i][j] == 0)
					tileManager->drawTile("Green", "Basic");
				if (tileModes[i][j] == 1)
					tileManager->drawTile("Red", "Basic");
				if (tileModes[i][j] == 2)
					tileManager->drawTile("Yellow", "Basic");
			MatrixManager::getInstance()->popMatrix4(MODELVIEW);
			MatrixManager::getInstance()->popMatrix3(NORMAL);
		}
	}
}