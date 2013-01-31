#ifndef LIGHT_H
#define LIGHT_H

#include <string>
using namespace std;

class Light
{
protected:
	float m_nColor[3];
	float m_nAmbient;
	float m_nDiffuse;
	float m_nSpecular;
	bool m_bEnabled;

public:
	void setAmbient(float nAmt);
	void setDiffuse(float nAmt);
	void setSpecular(float nAmt);
	float getDiffuse();
	float getAmbient();
	float getSpecular();
	void setColor(float red, float green, float blue);
	float *getColor();
	bool isEnabled();
	void setEnabled(bool bEnabled);
	virtual void sendToShader(string sShader) = 0;
};

#endif