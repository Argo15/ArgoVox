#include <GL/freeglut.h>
#include <math.h>
#include "Camera.h"
#include "GameState.h"
#include "InputManager.h"

#define PI 3.141592653589793

Camera::Camera()
{
	m_nHAngle=-(float)PI/4.0f;
    m_nVAngle=(float)PI/4.0f;
	m_nEyePos[0]=5.0; m_nEyePos[1]=5.0; m_nEyePos[2]=5.0; 
	recalculate();
	m_nRotSpeed=0.8f;
	m_nLastMouseX = -1;
	m_nLastMouseY = -1;
}

void Camera::mouseRotate()
{
	int nMouseX=InputManager::getInstance()->getMouseX();
	int nMouseY=InputManager::getInstance()->getMouseY();

	if (m_nLastMouseX == -1)
	{
		m_nLastMouseX = nMouseX;
		m_nLastMouseY = nMouseY;
		return;
	}

	int dx = nMouseX - m_nLastMouseX;
	int dy = nMouseY - m_nLastMouseY;

	m_nHAngle-=(float)dx*m_nRotSpeed*0.005f;
	m_nVAngle+=(float)dy*m_nRotSpeed*0.005f;

	if (m_nVAngle>0.999f*PI/2.0f)
	{
        m_nVAngle=0.999f*(float)PI/2.0f;
	}
    if (m_nVAngle<-0.999f*PI/2.0f)
	{
        m_nVAngle=-0.999f*(float)PI/2.0f;
	}

	recalculate();

	m_nLastMouseX = nMouseX;
	m_nLastMouseY = nMouseY;
	
	if (GameState::GAMESTATE->mouseHidden()) 
	{
		if (nMouseX>500) 
		{
			glutWarpPointer(300, nMouseY);
			m_nLastMouseX = 300;
		}
		if (nMouseX<300) 
		{
			glutWarpPointer(500, nMouseY);
			m_nLastMouseX = 500;
		}
		if (nMouseY>500) 
		{
			glutWarpPointer(nMouseX, 300);
			m_nLastMouseY = 300;
		}
		if (nMouseY<300) 
		{
			glutWarpPointer(nMouseX, 500);
			m_nLastMouseY = 500;
		}
	}
}

void Camera::move(float nSpeed)
{
	if (InputManager::getInstance()->isKeyDown('w'))
	{
		moveForward(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('s'))
	{
		moveBackward(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('a'))
	{
		moveLeft(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('d'))
	{
		moveRight(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('e'))
	{
		moveUp(nSpeed*0.1f);
	}
	if (InputManager::getInstance()->isKeyDown('q'))
	{
		moveDown(nSpeed*0.1f);
	}

	mouseRotate();
}

void Camera::moveForward(float nSpeed)
{
	m_nEyePos[0]-=cos(m_nHAngle)*nSpeed;
	m_nEyePos[2]+=sin(m_nHAngle)*nSpeed;
	m_nLookAt[0]-=cos(m_nHAngle)*nSpeed;
	m_nLookAt[2]+=sin(m_nHAngle)*nSpeed;
}

void Camera::moveBackward(float nSpeed)
{
	m_nEyePos[0]+=cos(m_nHAngle)*nSpeed;
	m_nEyePos[2]-=sin(m_nHAngle)*nSpeed;
	m_nLookAt[0]+=cos(m_nHAngle)*nSpeed;
	m_nLookAt[2]-=sin(m_nHAngle)*nSpeed;
}

void Camera::moveLeft(float nSpeed)
{
	m_nEyePos[2]+=cos(m_nHAngle)*nSpeed;
	m_nEyePos[0]+=sin(m_nHAngle)*nSpeed;
	m_nLookAt[2]+=cos(m_nHAngle)*nSpeed;
	m_nLookAt[0]+=sin(m_nHAngle)*nSpeed;
}

void Camera::moveRight(float nSpeed)
{
	m_nEyePos[2]-=cos(m_nHAngle)*nSpeed;
	m_nEyePos[0]-=sin(m_nHAngle)*nSpeed;
	m_nLookAt[2]-=cos(m_nHAngle)*nSpeed;
	m_nLookAt[0]-=sin(m_nHAngle)*nSpeed;
}

void Camera::moveUp(float nSpeed)
{
	m_nEyePos[1]+=nSpeed;
	m_nLookAt[1]+=nSpeed;
}

void Camera::moveDown(float nSpeed)
{
	m_nEyePos[1]-=nSpeed;
	m_nLookAt[1]-=nSpeed;
}

void Camera::recalculate()
{
	m_nLookAt[0]=m_nEyePos[0]-cos(m_nHAngle)*abs(cos(m_nVAngle));
	m_nLookAt[1]=m_nEyePos[1]-sin(m_nVAngle);
	m_nLookAt[2]=m_nEyePos[2]+sin(m_nHAngle)*abs(cos(m_nVAngle));
	m_nUp[0]=cos(m_nHAngle)*cos(m_nVAngle+(float)PI/2);
	m_nUp[1]=sin(m_nVAngle+(float)PI/2);
	m_nUp[2]=-sin(m_nHAngle)*cos(m_nVAngle+(float)PI/2);
	Vector3 vLook(m_nLookAt[0]-m_nEyePos[0],m_nLookAt[1]-m_nEyePos[1],m_nLookAt[2]-m_nEyePos[2]);
	Vector3 vUp(m_nUp[0],m_nUp[1],m_nUp[2]);
	Vector3 vRight = vLook.cross(vUp);
	vRight.normalize();
	m_nRight[0]=vRight[0];
	m_nRight[1]=vRight[1];
	m_nRight[2]=vRight[2];
}

void Camera::transform()
{
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::lookAt(
		glm::vec3(m_nEyePos[0],m_nEyePos[1],m_nEyePos[2]),
		glm::vec3(m_nLookAt[0],m_nLookAt[1],m_nLookAt[2]),
		glm::vec3(m_nUp[0],m_nUp[1],m_nUp[2])
	));
}

glm::mat4 Camera::transformToMatrix(glm::mat4 matrix)
{
	return matrix * glm::lookAt(
		glm::vec3(m_nEyePos[0],m_nEyePos[1],m_nEyePos[2]),
		glm::vec3(m_nLookAt[0],m_nLookAt[1],m_nLookAt[2]),
		glm::vec3(m_nUp[0],m_nUp[1],m_nUp[2])
	);
}


void Camera::setPosition(float x, float y, float z)
{
	m_nEyePos[0]=x;
	m_nEyePos[1]=y;
	m_nEyePos[2]=z;
}

void Camera::setLookAt(float x, float y, float z) 
{
	m_nLookAt[0]=x;
	m_nLookAt[1]=y;
	m_nLookAt[2]=z;
}

void Camera::setUp(float x, float y, float z) 
{
	m_nUp[0]=x;
	m_nUp[1]=y;
	m_nUp[2]=z;
}

void Camera::setRight(float x, float y, float z) 
{
	m_nRight[0]=x;
	m_nRight[1]=y;
	m_nRight[2]=z;
}

void Camera::setRotSpeed(float nSpeed) 
{
	m_nRotSpeed=nSpeed;
}

float Camera::getEyeX() 
{
	return m_nEyePos[0];
}

float Camera::getEyeY() 
{
	return m_nEyePos[1];
}

float Camera::getEyeZ() 
{
	return m_nEyePos[2];
}

float *Camera::getEye() 
{
	float *peye = m_nEyePos; 
	return peye;
}

Vector3 Camera::getEyeV() 
{
	return Vector3(m_nEyePos[0],m_nEyePos[1],m_nEyePos[2]);
}

Vector3 Camera::getLookAt() 
{
	return Vector3(m_nLookAt[0],m_nLookAt[1],m_nLookAt[2]);
}

Vector3 Camera::getUp() 
{
	return Vector3(m_nUp[0],m_nUp[1],m_nUp[2]);
}

Vector3 Camera::getRight() 
{
	return Vector3(m_nRight[0],m_nRight[1],m_nRight[2]);
}