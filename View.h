#ifndef VIEW_H
#define VIEW_H

#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

class View
{
private:
	double m_nFov, m_nAspect, m_nZNear, m_nZFar;
	double m_nLeft, m_nRight, m_nTop, m_nBottom, m_nFront, m_nBack;
	int width, height;

public:
	void use3D(bool use3D);
	void set3D(GLdouble nFov, GLdouble nAspect, GLdouble nZNear, GLdouble nZFar);
	void set2D(GLdouble nLeft, GLdouble nRight, GLdouble nBottom, GLdouble nTop, GLdouble nFront, GLdouble nBack);
	void viewport();
	void viewport(int x, int y, int width, int height);
	double getFOV();
	double getAspect();
	double getNear();
	double getFar();
	double getOrthoWidth();
	double getOrthoHeight();
	double getOrthoDepth();
	double getOrthoNear();
	double getOrthoFar();
};

#endif