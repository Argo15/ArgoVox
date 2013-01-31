#ifndef SCENETILES_H
#define SCENETILES_H

#include "TileManager.h"

enum TileMode{ GREEN, RED, YELLOW };

class SceneTiles {
private:
	int tileModes[10][10];
	bool updatable[10][10];
	TileManager *tileManager;

	int selectedX, selectedZ;

public:
	SceneTiles();

	int *getTileModes() {return &tileModes[0][0];}
	int getTileMode(int x, int y) {return tileModes[x][y];}
	void setTileMode(int x, int y, int value) {tileModes[x][y] = value;}
	void updateTiles();
	void clearUpdatables();
	void draw();
};

#endif