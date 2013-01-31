#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <string>
#include <map>
#include "Material.h"
using namespace std;

class MaterialManager {
private:
	static MaterialManager *m_pInstance;
	map<string,Material *> m_materials;
	string getSafeName(string baseName);

	MaterialManager();
public:
	static MaterialManager *getInstance();

    void initialize();
	void loadMaterial(string sFileName);
	string addMaterialSafe(Material *material);
	bool hasMaterial(string sName);
	string renameMaterial(string sCurrentName, string sNewName);
	void deleteMaterial(string sName);
	void deleteAllMaterials();

	void saveAllMaterials();

	void useMaterial(string sName);
	Material *getMaterial(string sName);
	map<string,Material *> *getMaterials();
};

#endif