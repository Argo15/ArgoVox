#include "Actor.h"
#include "MatrixManager.h"
#include "ShaderManager.h"
#include "MaterialManager.h"

Actor::Actor(string *sModel, string *sMaterial) : Transformable()
{
	this->m_psModel = sModel;
	this->m_psMaterial = sMaterial;
}

void Actor::drawActor(string sShader)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(sShader);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	if (sShader == "Basic" || sShader == "GBuffer" || sShader == "BuildVoxels") 
	{
		glslProgram->sendUniform("normalMatrix", &MatrixManager::getInstance()->getMatrix3(NORMAL)[0][0],false,3);
		MaterialManager::getInstance()->getMaterial(*m_psMaterial)->sendToShader(sShader);
	}
	ModelManager::getInstance()->getModel(*m_psModel)->draw();
}

float Actor::getScaledRadius()
{
	return ModelManager::getInstance()->getModel(*m_psModel)->getRadius()*max(max(m_nScale[0],m_nScale[1]),m_nScale[2]);
}

void Actor::setModel(string *sModel) 
{
	this->m_psModel = sModel;
}

string *Actor::getModel() 
{
	return m_psModel;
}

void Actor::setMaterial(string *sMaterial) 
{
	this->m_psMaterial = sMaterial;
}

string *Actor::getMaterial() 
{
	return m_psMaterial;
}