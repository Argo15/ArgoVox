#ifndef DIRECTLIGHT_H
#define DIRECTLIGHT_H

#include <string>
#include "Transformable.h"
#include "Light.h"
#include "GLSLProgram.h"
using namespace std;

class DirectLight : public Light, public Transformable
{
private:
	float m_nDirection[4];

public:
	DirectLight();

	void sendToShader(string sShader);
	void setDirection(float nDirX, float nDirY, float nDirZ);
	Vector3 getDirection();
	Vector3 getUp();
	Vector3 getRight();
};

#endif