#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include <fstream>
#include "Entity.h"
#include "TextureManager.h"
#include "Matrix.h"
#include "ShaderManager.h"
using namespace std;

struct SaveMat 
{
	float nColor[3];
	float nEmission[3];
	float nSpecular;
	int nShininess;

	float nTexOffset[2];
	float nTexScale[2];
	float nTexRotate;

	char sName[100];
	char sTextureName[100];
	char sNormalName[100];

	bool bNormalEnabled;
};

class Material : public Entity
{
private:
	float m_nColor[3];
	float m_nEmission[3];
	float m_nSpecular;
	int m_nShininess;

	float m_nTexOffset[2];
	float m_nTexScale[2];
	float m_nTexRotate;

	string *m_sTextureName;
	string *m_sNormalName;

	bool m_bNormalEnabled;
public:
	Material();
	Material(Material *copyMaterial);

	void use();
	void sendToShader(string sShader);

	bool loadMaterial(const char* sFileName);
	void saveMaterial(const char* sFileName);

	void setColor(float nRed, float nGreen, float nBlue);
	void setEmission(float nRed, float nGreen, float nBlue);
	void setSpecular(float nIntensity);
	void setShine(int nShine);
	void setTexOffset(float nX, float nY);
	void setUTexOffset(float nValue);
	void setVTexOffset(float nValue);
	void setTexScale(float nX, float nY);
	void setUTexScale(float nValue);
	void setVTexScale(float nValue);
	void setTexRotate(float nRotate);
	float *getColor();
	float *getEmission();
	float getSpecular();
	int getShine();
	float *getTexOffset();
	float *getTexScale();
	float getTexRotate();
	void setTexture(string *sTextureName);
	string *getTexture();
	void setNormal(string *sNormalName);
	string *getNormal();
	void enableNormal(bool bEnabled);
	bool normalsEnabled();
};

#endif