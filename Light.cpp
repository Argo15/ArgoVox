#include "Light.h"

void Light::setAmbient(float nAmt)
{
	m_nAmbient=nAmt;
}

void Light::setDiffuse(float nAmt)
{
	m_nDiffuse=nAmt;
}

void Light::setSpecular(float nAmt)
{
	m_nSpecular=nAmt;
}

float Light::getDiffuse()
{
	return m_nDiffuse;
}

float Light::getAmbient()
{
	return m_nAmbient;
}

float Light::getSpecular() 
{
	return m_nSpecular;
}

void Light::setColor(float red, float green, float blue) 
{
	m_nColor[0]=red; 
	m_nColor[1]=green; 
	m_nColor[2]=blue;
}

float *Light::getColor() 
{
	return m_nColor;
}

bool Light::isEnabled() 
{
	return m_bEnabled;
}

void Light::setEnabled(bool bEnabled) 
{
	this->m_bEnabled=bEnabled;
}