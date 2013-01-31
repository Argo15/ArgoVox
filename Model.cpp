#include "Model.h"

int Model::getNumTriangles()
{
	return m_nNumTriangles;
}

float *Model::getCenter() 
{
	return m_nCenter;
}

float Model::getRadius() 
{
	return m_nRadius;
}