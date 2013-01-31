#include "MaterialManager.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include "dirent.h"
#include "Logger.h"

MaterialManager* MaterialManager::m_pInstance = 0;

MaterialManager::MaterialManager()
{
	
}

MaterialManager *MaterialManager::getInstance() 
{
	if (m_pInstance == 0) 
	{
		m_pInstance = new MaterialManager();
	}
	return m_pInstance;
}

void MaterialManager::initialize()
{
	Logging::GRAPHICS->info("Loading Materials");
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("./Data/Materials") ){
		while(entry = readdir(pDIR)){
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
				loadMaterial(string("./Data/Materials/") + entry->d_name);
		}
		closedir(pDIR);
	}
	Logging::GRAPHICS->info("Done Loading Materials");
}

void MaterialManager::loadMaterial(string sFileName)
{
	Material *newMaterial = new Material();
	if (newMaterial->loadMaterial(sFileName.c_str())) 
	{
		string *sName = newMaterial->getName();
		m_materials[*sName] = newMaterial;
	} 
	else 
	{
		Logging::GRAPHICS->error("Material failed to load: " + sFileName);
	}
}

/* 
 * returns a string that is not currently being used
 */
string MaterialManager::getSafeName(string baseName)
{
	if (!hasMaterial(baseName))
	{
		return baseName;
	} 
	else
	{
		int nNum = 1;
		while (true)
		{
			char buff[10];
			itoa(nNum, buff, 10);
			string sNum = string(buff);
			string newName = baseName + sNum;
			if (!hasMaterial(newName)) 
			{
				return newName;
			}
			nNum++;
		}
	}
}

/* returns the name of the material */
string MaterialManager::addMaterialSafe(Material *material)
{
	string matName = getSafeName(*(material->getName()));
	material->setName(matName);
	m_materials[matName] = material;
	return matName;
}

bool MaterialManager::hasMaterial(string sName)
{
	if (m_materials.find(sName) != m_materials.end()) 
	{
		return true;
	}
	return false;
}

string MaterialManager::renameMaterial(string sCurrentName, string sNewName) 
{
	Material *mat = m_materials[sCurrentName];
	m_materials.erase(sCurrentName);
	sNewName = getSafeName(sNewName);
	mat->setName(sNewName);
	m_materials[sNewName] = mat;
	return sNewName;
}

void MaterialManager::deleteMaterial(string sName)
{
	if (m_materials.find(sName) != m_materials.end()) 
	{
		delete m_materials[sName];
		m_materials.erase(sName);
	}
}

void MaterialManager::deleteAllMaterials()
{
	m_materials.clear();
}

void MaterialManager::saveAllMaterials()
{
	for (std::map<std::string, Material *>::iterator it = m_materials.begin(); it != m_materials.end(); it++)
	{
		Material *mat = (*it).second;
		string sFileName = "Data/Materials/" + (*it).first + ".amtl";
		mat->saveMaterial(sFileName.c_str());
	}
}

void MaterialManager::useMaterial(string sName) 
{
	m_materials[sName]->use();
}

Material *MaterialManager::getMaterial(string sName) 
{
	return m_materials[sName];
}

map<string,Material *> *MaterialManager::getMaterials() 
{
	return &m_materials;
}