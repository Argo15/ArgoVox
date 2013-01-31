#ifndef PERLINNOISE_H
#define PERLINNOISE_H

#include "Texture.h"
#include <math.h>
#include <time.h>
#include <GL/glu.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>

class PerlinNoise : public Texture
{
private:
	float m_nMap32[32 * 32];
	float m_nMap256[256 * 256];

	float noise(int nX, int nY, int nRandom);
	void setNoise(float  *nMap);
	float interpolate(float nX, float nY, float  *nMap);
	void overlapOctaves(float  *nMap32, float  *nMap256);
	void expFilter(float  *nMap);

public:
	bool load(const char *sName);
};

#endif