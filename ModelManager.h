#ifndef MODELMANAGER_H
#define MODELMANAGER_H

#include <string>
#include <map>
#include "AObjModel.h"
using namespace std;

class ModelManager
{
private:
	static ModelManager *m_pInstance;
	map<string,Model *> m_models;

	ModelManager();
public:
	static ModelManager *getInstance();

    void initialize();
	void loadModel(string sFileName);
	void drawModel(string sName);
	void drawModelGeometry(string sName);
	void deleteModel(string sName);
	void deleteAllModels();

	Model *getModel(string sName);
	map<string,Model *> *getModels();
};

#endif