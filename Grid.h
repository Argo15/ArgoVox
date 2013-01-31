#ifndef GRID_H
#define GRID_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>

class Grid{
private:
	float m_nColor[3];
	int m_nLength, m_nWidth;
	float m_nCenter[2];

public:
	Grid();
	Grid(int nLength, int nWidth);

	void setColor(float nRed, float nGreen, float nBlue);
	void setSize(int nLength, int nWidth);
	void setCenter(float nX, float nY);
	void draw();
};
#endif
