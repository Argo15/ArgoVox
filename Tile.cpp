#include "Tile.h"

Tile::Tile() : Chunk() 
{

}

bool Tile::loadTile(string sFilename) 
{
	return loadChunk(sFilename);
}

void Tile::drawTile(string sShader) 
{
	drawChunk(sShader);
}