#ifndef UNPROJECT_H
#define UNPROJECT_H

#include <QtOpenGL>
#include "Matrix.h"

#define PI 3.1415

static Vector3 getUnproject(double depth,int mouseX, int mouseY){
	Vector3 finalPos;
	GLdouble result[3];
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	
	glGetIntegerv(GL_VIEWPORT,viewport);
	glGetDoublev(GL_MODELVIEW_MATRIX,mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX,projmatrix);

	gluUnProject((GLdouble)mouseX,(GLdouble)(viewport[3]-mouseY),(GLdouble)depth,
					mvmatrix,projmatrix,viewport,
					&result[0],&result[1],&result[2]);
	finalPos[0]=result[0];
	finalPos[1]=result[1];
	finalPos[2]=result[2];
	return finalPos;
}

static Vector3 getDragPoint(Vector3 *dragLine, Vector3 *unProjLine)
{
	Vector3 P0(dragLine[0]);
	Vector3 Vp(dragLine[1]);
	Vector3 Q0(unProjLine[0]);
	Vector3 Vq(unProjLine[1]);
	float a;
	float b[2];
	float c[2];
	float s;

	Vp.normalize();
	Vq.normalize();

	a=1.0/(1.0-(Vp.dot(Vq)*Vp.dot(Vq)));

	b[0]=a;
	b[1]=Vp.dot(Vq)*a;

	c[0]=Vector3(Q0-P0).dot(Vp);
	c[1]=Vector3(P0-Q0).dot(Vq);

	s=b[0]*c[0]+b[1]*c[1];

	Vector3 dragPoint(P0+Vp*s);

	return dragPoint;
}

static float getRotAngle(Vector3 *unProjLine, Vector3 *normal, Vector3 *tangent)
{
	float t;

	t = -(normal->dot(unProjLine[0]))/normal->dot(unProjLine[1]);

	Vector3 intersectPoint = unProjLine[0]+unProjLine[1]*t;
	intersectPoint.normalize();
	tangent->normalize();

	float angle = acos(intersectPoint.dot(*tangent));

	Vector3 cross = intersectPoint.cross(*tangent);
	cross.normalize();
	normal->normalize();
	if (cross.dot(*normal)>0.01)
		angle*=-1;

	return angle+PI/2;
}

#endif