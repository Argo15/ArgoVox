#ifndef ACTOR_H
#define ACTOR_H

#include <math.h>
#include "Model.h"
#include "Matrix.h"
#include "ModelManager.h"
#include "Transformable.h"
#include "GLSLProgram.h"

class Actor : public Entity, public Transformable 
{
private:
	string *m_psModel;
	string *m_psMaterial;
	
public:
	Actor(string *sModel = 0, string *sMaterial = 0);

	void drawActor(string sShader);
	float getScaledRadius();
	void setModel(string *sModel);
	string *getModel();
	void setMaterial(string *sMaterial);
	string *getMaterial();

};

#endif