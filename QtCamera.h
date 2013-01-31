#ifndef QTCAMERA_H
#define QTCAMERA_H

#include "Camera.h"

class QtCamera : public Camera {
private:

public:
	virtual void move(float speed);
};

#endif