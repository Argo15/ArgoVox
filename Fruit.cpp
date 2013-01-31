#include "Fruit.h"

Fruit::Fruit(std::string sWorldTile, std::string sInventoryTexture, std::string sGroundTile) : Object()
{
	setWorldTile(sWorldTile);
	setInventoryTexture(sInventoryTexture);
	setGroundTile(sGroundTile);
}