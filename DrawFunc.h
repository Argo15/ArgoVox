#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include "MatrixManager.h"

static void drawScreen(float nX1, float nY1, float nX2, float nY2)
{
	glBegin(GL_QUADS);
		glTexCoord2f(0,0);		 glVertex2f(nX1,nY1);
		glTexCoord2f(1.0f,0);	 glVertex2f(nX2,nY1);
		glTexCoord2f(1.0f,1.0f); glVertex2f(nX2,nY2);
		glTexCoord2f(0,1.0f);	 glVertex2f(nX1,nY2);
	glEnd();
}

static void drawScreenShader(float nX1, float nY1, float nX2, float nY2)
{
	glBegin(GL_QUADS);
		glVertexAttrib2f(1,0,0);		glVertexAttrib2f(0,nX1,nY1);
		glVertexAttrib2f(1,1.0f,0);		glVertexAttrib2f(0,nX2,nY1);
		glVertexAttrib2f(1,1.0f,1.0f);	glVertexAttrib2f(0,nX2,nY2);
		glVertexAttrib2f(1,0,1.0f);		glVertexAttrib2f(0,nX1,nY2);
	glEnd();
}

static void transformNoShaders()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMultMatrixf(&MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glMultMatrixf(&MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
}