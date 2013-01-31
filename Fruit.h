#ifndef FRUIT_H
#define FRUIT_H

#include <string>
#include "Object.h"

/*
 * Author: wcrane
 * Date: 1/6/2013
 * Fruit object created by FruitFactory, appears on trees.
 */
class Fruit : public Object
{
public:
	Fruit(std::string sWorldTile, std::string sInventoryTexture, std::string sGroundTile);
};

#endif