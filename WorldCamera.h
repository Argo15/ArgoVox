#ifndef WORLDCAMERA_H
#define WORLDCAMERA_H

#include "Camera.h"

class WorldCamera : public Camera
{
private:
	float m_nSpeedFactor;

public:
	WorldCamera();

	void move(float nSpeed);
	void moveForward(float nSpeed);
	void moveBackward(float nSpeed);
	void moveLeft(float nSpeed);
	void moveRight(float nSpeed);
	void moveUp(float nSpeed);
	void moveDown(float nSpeed);
};

#endif