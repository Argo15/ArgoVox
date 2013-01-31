#ifndef OBJECT_H
#define OBJECT_H

#include <string>

/*
 * Author: wcrane
 * Date: 7/14/2012
 * Generic object that can be placed in your inventory, placed on the ground, or placed in the world/house as an interactive object.
 */
class Object
{
private:
	std::string m_sWorldTile;
	std::string m_sInventoryTexture;
	std::string m_sGroundTile;

public:
	Object();

	std::string getWorldTile();
	void setWorldTile(std::string sTile);

	std::string getInventoryTexture();
	void setInventoryTexture(std::string sTexture);

	std::string getGroundTile();
	void setGroundTile(std::string sTile);
};

#endif