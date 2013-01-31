#include "ModelManager.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include "dirent.h"
#include "Logger.h"

ModelManager* ModelManager::m_pInstance = 0;

ModelManager::ModelManager()
{
}

ModelManager *ModelManager::getInstance() 
{
	if (m_pInstance == 0) 
	{
		m_pInstance = new ModelManager();
	}
	return m_pInstance;
}

void ModelManager::initialize()
{
	Logging::GRAPHICS->info("Loading Models");
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("./Data/Models") )
	{
		while(entry = readdir(pDIR))
		{
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				loadModel(string("./Data/Models/") + entry->d_name);
			}
		}
		closedir(pDIR);
	}
	Logging::GRAPHICS->info("Done Loading Models");
}

void ModelManager::loadModel(string sFileName)
{
	Model *pModel = new AObjModel();
	if (pModel->load(sFileName)) 
	{
		int nStartPos = sFileName.rfind("/")+1;
		int nEndPos = sFileName.rfind(".");
		string sName = sFileName.substr(nStartPos,nEndPos-nStartPos);
		pModel->setName(sName);
		m_models[sName]=pModel;
	} 
	else 
	{
		Logging::GRAPHICS->error("Model failed to load: " + sFileName);
	}
}

void ModelManager::drawModel(string sName)
{
	m_models[sName]->draw();
}

void ModelManager::drawModelGeometry(string sName)
{
	m_models[sName]->drawGeometry();
}

void ModelManager::deleteModel(string sName)
{
	if (m_models.find(sName) == m_models.end())
	{
		m_models[sName]->remove();
		delete m_models[sName];
		m_models.erase(sName);
	}
}

void ModelManager::deleteAllModels()
{
	m_models.clear();
}

Model *ModelManager::getModel(string sName)
{ 
	return m_models[sName]; 
}

map<string,Model *> *ModelManager::getModels() 
{
	return &m_models;
}	