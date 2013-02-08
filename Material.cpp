#include "Material.h"
#include "TextureManager.h"
#include <fstream>

Material::Material()
	: Entity()
{
	for (int i=0;i<3;i++)
	{
		m_nColor[i]=1.0;
		m_nEmission[i]=0.0;
	}
	m_nTexOffset[0]=0.0;
	m_nTexOffset[1]=0.0;
	m_nTexScale[0]=1.0;
	m_nTexScale[1]=1.0;
	m_nTexRotate=0.0;
	m_nSpecular=0.0;
	m_nShininess=25;
	m_sNormalName=TextureManager::getInstance()->getTexture("White")->getName();
	m_bNormalEnabled=false;
}

Material::Material(Material *copyMaterial)
{
	*this=*copyMaterial;
}


void Material::use()
{
	float spec[] = {m_nSpecular, m_nSpecular, m_nSpecular};
	glDisable(GL_COLOR_MATERIAL);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,m_nColor);
	glMaterialfv(GL_FRONT,GL_AMBIENT,m_nColor);
	glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
	glMateriali(GL_FRONT,GL_SHININESS,m_nShininess);
	glMaterialfv(GL_FRONT,GL_EMISSION,m_nEmission);
	glMatrixMode(GL_TEXTURE);
	glActiveTextureARB(GL_TEXTURE0);
	glLoadIdentity();
	glTranslatef(m_nTexOffset[0],m_nTexOffset[1],0.0);
	glRotatef(m_nTexRotate,0.0,0.0,1.0);
	glScalef(m_nTexScale[0],m_nTexScale[1],1.0);
	glMatrixMode(GL_MODELVIEW);

	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	TextureManager::getInstance()->bindTexture(*m_sTextureName);
}

void Material::sendToShader(string sShader)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(sShader);
	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);
	TextureManager::getInstance()->bindTexture(*m_sTextureName);
	glActiveTextureARB(GL_TEXTURE1);
	glEnable(GL_TEXTURE_2D);
	TextureManager::getInstance()->bindTexture(*m_sNormalName);
	glslProgram->sendUniform("tex", 0);
	glslProgram->sendUniform("normalmap", 1);
	glslProgram->sendUniform("material.normalenabled", m_bNormalEnabled);
	glslProgram->sendUniform("material.color", m_nColor[0],m_nColor[1],m_nColor[2]);
	if (sShader == "GBuffer" || sShader == "Basic" || sShader == "BuildVoxels")
	{
		glslProgram->sendUniform("material.emission", m_nEmission[0],m_nEmission[1],m_nEmission[2]);
		glslProgram->sendUniform("material.specular", m_nSpecular);
		glslProgram->sendUniform("material.shininess", (float)m_nShininess/128.0f);
	}
	glm::mat4 textureMatrix = glm::mat4(1.0f);
	textureMatrix = glm::translate(textureMatrix, glm::vec3(m_nTexOffset[0],m_nTexOffset[1],0));
	textureMatrix = glm::rotate(textureMatrix, m_nTexRotate, glm::vec3(0,0,1.0));
	textureMatrix = glm::scale(textureMatrix, glm::vec3(m_nTexScale[0],m_nTexScale[1],1.0));
	glslProgram->sendUniform("textureMatrix", &textureMatrix[0][0]);
}

bool Material::loadMaterial(const char* sFileName) 
{
	SaveMat load;

	ifstream file(sFileName, ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{ 
		file.seekg (0, ios::beg);
		file.read((char*)&load,sizeof(load));
	} 
	else 
	{ 
		return false; 
	}
	file.close();

	for (int i=0; i<3; i++) 
	{
		m_nColor[i] = load.nColor[i];
		m_nEmission[i] = load.nEmission[i];
	}
	for (int i=0; i<2; i++) 
	{
		m_nTexOffset[i] = load.nTexOffset[i];
		m_nTexScale[i] = load.nTexScale[i];
	}
	m_nSpecular = load.nSpecular;
	m_nShininess = load.nShininess;
	m_nTexRotate = load.nTexRotate;
	m_bNormalEnabled = load.bNormalEnabled;

	m_sName = new string(load.sName);
	m_sTextureName = new string(load.sTextureName);
	m_sNormalName = new string(load.sNormalName);
	return true;
}

void Material::saveMaterial(const char *sFileName)
{
	SaveMat save;
	for (int i=0; i<3; i++) 
	{
		save.nColor[i] = m_nColor[i];
		save.nEmission[i] = m_nEmission[i];
	}
	for (int i=0; i<2; i++) 
	{
		save.nTexOffset[i] = m_nTexOffset[i];
		save.nTexScale[i] = m_nTexScale[i];
	}
	save.nSpecular = m_nSpecular;
	save.nShininess = m_nShininess;
	save.nTexRotate = m_nTexRotate;
	save.bNormalEnabled = m_bNormalEnabled;

	strcpy(save.sName, m_sName->c_str());
	strcpy(save.sTextureName, m_sTextureName->c_str());
	strcpy(save.sNormalName, m_sNormalName->c_str());

	ofstream file(sFileName, ios::out|ios::binary|ios::ate);
	if (file.is_open())
	{ 
		file.write((char*)&save,sizeof(save));
	}
	file.close();
}

void Material::setColor(float nRed, float nGreen, float nBlue) 
{
	m_nColor[0] = nRed;
	m_nColor[1] = nGreen;
	m_nColor[2] = nBlue;
}

void Material::setEmission(float nRed, float nGreen, float nBlue) 
{
	m_nEmission[0] = nRed;
	m_nEmission[1] = nGreen;
	m_nEmission[2] = nBlue;
}

void Material::setSpecular(float nIntensity) 
{
	m_nSpecular = nIntensity;
}

void Material::setShine(int nShine) 
{
	m_nShininess = nShine;
}

void Material::setTexOffset(float nX, float nY) 
{
	m_nTexOffset[0] = nX; 
	m_nTexOffset[1] = nY;
}

void Material::setUTexOffset(float nValue) 
{
	m_nTexOffset[0] = nValue;
}

void Material::setVTexOffset(float nValue) 
{
	m_nTexOffset[1] = nValue;
}

void Material::setTexScale(float nX, float nY) 
{
	m_nTexScale[0] = nX; 
	m_nTexScale[1] = nY;
}

void Material::setUTexScale(float nValue) 
{
	m_nTexScale[0] = nValue;
}

void Material::setVTexScale(float nValue) 
{
	m_nTexScale[1] = nValue;
}

void Material::setTexRotate(float nRotate) 
{
	m_nTexRotate = nRotate;
}

float *Material::getColor() 
{
	return m_nColor;
}

float *Material::getEmission() 
{
	return m_nEmission;
}

float Material::getSpecular() 
{
	return m_nSpecular;
}

int Material::getShine() 
{
	return m_nShininess;
}

float *Material::getTexOffset() 
{
	return m_nTexOffset;
}

float *Material::getTexScale() 
{
	return m_nTexScale;
}

float Material::getTexRotate()
{
	return m_nTexRotate;
}

void Material::setTexture(string *sTextureName) 
{
	m_sTextureName = sTextureName;
}

string *Material::getTexture()
{
	return m_sTextureName;
}

void Material::setNormal(string *sNormalName) 
{
	m_sNormalName = sNormalName;
}

string *Material::getNormal() 
{
	return m_sNormalName;
}

void Material::enableNormal(bool bEnabled) 
{
	m_bNormalEnabled = bEnabled;
}

bool Material::normalsEnabled()
{
	return m_bNormalEnabled;
}