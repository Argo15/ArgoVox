#include "View.h"
#include "Matrix.h"
#include "MatrixManager.h"

void View::use3D(bool use3D)
{
    if (use3D)
	{
		MatrixManager::getInstance()->putMatrix4(PROJECTION, (glm::mat4) glm::perspective(m_nFov, m_nAspect, m_nZNear, m_nZFar));
	}
    else
	{
		MatrixManager::getInstance()->putMatrix4(PROJECTION, (glm::mat4) glm::ortho(m_nLeft, m_nRight, m_nBottom, m_nTop, m_nFront, m_nBack));
	}
}

void View::set3D(GLdouble nFov, GLdouble nAspect, GLdouble nZNear, GLdouble nZFar)
{
	m_nFov=nFov;
    m_nAspect=nAspect;
    m_nZNear=nZNear;
    m_nZFar=nZFar;
}

void View::set2D(GLdouble nLeft, GLdouble nRight, GLdouble nBottom, GLdouble nTop, GLdouble nFront, GLdouble nBack)
{
        m_nLeft=nLeft;
        m_nRight=nRight;
        m_nBottom=nBottom;
        m_nTop=nTop;
        m_nFront=nFront;
        m_nBack=nBack;
}

void View::viewport() 
{
	glViewport(0,0,width,height);
}

void View::viewport(int x, int y, int width, int height) 
{
	this->width = width; 
	this->height = height; 
	glViewport(x,y,width,height);
}

double View::getFOV() 
{
	return m_nFov;
}

double View::getAspect() 
{
	return m_nAspect;
}

double View::getNear() 
{
	return m_nZNear;
}

double View::getFar() 
{
	return m_nZFar;
}

double View::getOrthoWidth() 
{
	return m_nRight-m_nLeft;
}

double View::getOrthoHeight() 
{
	return m_nTop-m_nBottom;
}

double View::getOrthoDepth() 
{
	return m_nBack-m_nFront;
}

double View::getOrthoNear()
{
	return m_nFront;
}

double View::getOrthoFar() 
{
	return m_nBack;
}