#include "Grid.h"

Grid::Grid()
{
	m_nWidth=16;
	m_nLength=16;
	m_nColor[0]=0;
	m_nColor[1]=0;
	m_nColor[2]=0;
	m_nCenter[0]=0;
	m_nCenter[1]=0;
}

Grid::Grid(int nLength, int nWidth)
{
	m_nLength=nLength;
	m_nWidth=nWidth;
	m_nColor[0]=0;
	m_nColor[1]=0;
	m_nColor[2]=0;
	m_nCenter[0]=0;
	m_nCenter[1]=0;
}

void Grid::setColor(float nRed, float nGreen, float nBlue)
{
	m_nColor[0]=nRed;
	m_nColor[1]=nGreen;
	m_nColor[2]=nBlue;
}

void Grid::setSize(int nLength, int nWidth)
{
	m_nLength=nLength;
	m_nWidth=nWidth;
}

void Grid::setCenter(float nX, float nY)
{
	m_nCenter[0]=nX;
	m_nCenter[1]=nY;
}

void Grid::draw()
{
	float nU = (float)m_nWidth/2.0f;
	float nV = (float)m_nLength/2.0f;
	glLineWidth(2.0);
	glVertexAttrib3f(2,0.0,0.0,0.0);
	glVertexAttrib3f(3,0.0,0.0,0.0);
	glVertexAttrib3f(4,0.0,0.0,0.0);
	//glVertexAttrib3f(5,m_nColor[0],m_nColor[1],m_nColor[2]);
	glColor3f(1.0,1.0,1.0);
	for (int x=0;x<=m_nWidth;x++)
	{
		float a = m_nCenter[0]-nU+(float)x;
		for (float i=-nV; i<nV-0.1; i+=0.1f)
		{
			glBegin(GL_LINES);
				glVertex3f(a,0,m_nCenter[1]+i);
				glVertex3f(a,0,m_nCenter[1]+i+0.1);
			glEnd();
		}
	}
	for (int y=0;y<=m_nLength;y++)
	{
		for (float i=-nU; i<nU-0.1; i+=0.1f)
		{
			float b = m_nCenter[1]-nV+(float)y;
			glBegin(GL_LINES);
				glVertex3f(m_nCenter[0]+i,0,b);
				glVertex3f(m_nCenter[0]+i+0.1,0,b);
			glEnd();
		}
	}
	glLineWidth(1.0);
}