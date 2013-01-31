#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <Gl/gl.h>
#include <GL/glu.h>
#include "Matrix.h"
#include "MatrixManager.h"

class Camera
{
protected:
	float m_nEyePos[3];
	float m_nLookAt[3];
	float m_nUp[3];
	float m_nRight[3];
	float m_nHAngle;
	float m_nVAngle;
	int m_nLastMouseX, m_nLastMouseY;

	float m_nRotSpeed;

public:
	Camera();

	void mouseRotate();

	virtual void move(float nSpeed);
	virtual void moveForward(float nSpeed);
	virtual void moveBackward(float nSpeed);
	virtual void moveLeft(float nSpeed);
	virtual void moveRight(float nSpeed);
	virtual void moveUp(float nSpeed);
	virtual void moveDown(float nSpeed);

	void recalculate();

	void transform();
	glm::mat4 transformToMatrix(glm::mat4 matrix);

	void setPosition(float x, float y, float z);
	void setLookAt(float x, float y, float z);
	void setUp(float x, float y, float z);
	void setRight(float x, float y, float z);
	void setRotSpeed(float nSpeed);
	float getEyeX();
	float getEyeY();
	float getEyeZ();
	float *getEye();
	Vector3 getEyeV();
	Vector3 getLookAt();
	Vector3 getUp();
	Vector3 getRight();
};

#endif
