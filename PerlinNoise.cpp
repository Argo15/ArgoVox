#include "PerlinNoise.h"

float PerlinNoise::noise(int nX, int nY, int nRandom)
{
	int n = nX + nY * 57 + nRandom * 131;
	n = (n<<13) ^ n;
	return (1.0f - ( (n * (n * n * 15731 + 789221) +
		1376312589)&0x7fffffff)* 0.000000000931322574615478515625f);
}

void PerlinNoise::setNoise(float  *nMap)
{
	float temp[34][34];
	int nRandom=rand() % 5000;

	for (int y=1; y<33; y++)
	{
		for (int x=1; x<33; x++)
		{
			temp[x][y] = 128.0f + noise(x,  y,  nRandom)*128.0f;
		}
	}
	for (int x=1; x<33; x++)
	{
		temp[0][x] = temp[32][x];
		temp[33][x] = temp[1][x];
		temp[x][0] = temp[x][32];
		temp[x][33] = temp[x][1];
	}
	temp[0][0] = temp[32][32];
	temp[33][33] = temp[1][1];
	temp[0][33] = temp[32][1];
	temp[33][0] = temp[1][32];

	for (int y=1; y<33; y++)
	{
		for (int x=1; x<33; x++)
		{
			float center = temp[x][y]/4.0f;
			float sides = (temp[x+1][y] + temp[x-1][y] + temp[x][y+1] + temp[x][y-1])/8.0f;
			float corners = (temp[x+1][y+1] + temp[x+1][y-1] + temp[x-1][y+1] + temp[x-1][y-1])/16.0f;

			m_nMap32[((x-1)*32) + (y-1)] = center + sides + corners;
		}
	}
}

float PerlinNoise::interpolate(float nX, float nY, float  *nMap)
{
	int Xint = (int)nX;
	int Yint = (int)nY;

	float Xfrac = nX - Xint;
	float Yfrac = nY - Yint;
	int X0 = Xint % 32;
	int Y0 = Yint % 32;
	int X1 = (Xint + 1) % 32;
	int Y1 = (Yint + 1) % 32;
	float bot = nMap[X0*32 + Y0] + Xfrac * (nMap[X1*32 + Y0] - nMap[X0*32 + Y0]);
	float top = nMap[X0*32 + Y1] + Xfrac * (nMap[X1*32 +  Y1] - nMap[X0*32 + Y1]);

	return (bot + Yfrac * (top - bot));
}

void PerlinNoise::overlapOctaves(float  *nMap32, float  *nMap256)
{
	for (int x=0; x<256*256; x++)
	{
		nMap256[x] = 0;
	}
	for (int octave=0; octave<4; octave++)
	{
		for (int x=0; x<256; x++)
		{
			for (int y=0; y<256; y++)
			{
				float scale = 1 / pow(2.0, 3-octave);
				float noise = interpolate(x*scale, y*scale , nMap32);
				nMap256[(y*256) + x] += noise / pow(2.0, octave);
			}
		}
	}
}

void PerlinNoise::expFilter(float  *nMap)
{
  float cover = 20.0f;
  float sharpness = 0.95f;

  for (int x=0; x<256*256; x++)
  {
    float c = nMap[x] - (255.0f-cover);
    if (c<0)     c = 0;
    nMap[x] = 255.0f - ((float)(pow(sharpness, c))*255.0f);
  }
}

bool PerlinNoise::load(const char *sName)
{
	srand ( time(NULL) );
	setNoise(m_nMap32);
	overlapOctaves(m_nMap32, m_nMap256);
	expFilter(m_nMap256);                   //Our cloud function  

	char texture[256][256][3];       //Temporary array to hold texture RGB values 

	for(int i=0; i<256; i++)       //Set cloud color value to temporary array
	{
		for(int j=0; j<256; j++) 
		{
			float color = m_nMap256[i*256+j]; 
			texture[i][j][0]=(char)color;
			texture[i][j][1]=(char)color;
			texture[i][j][2]=(char)color;
		}
	}
                  
	glGenTextures(1, &m_nTextureID);
	glBindTexture(GL_TEXTURE_2D, m_nTextureID);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, 256, 256, GL_RGB, GL_UNSIGNED_BYTE, texture);

	this->setName(string(sName));

	return true;
}