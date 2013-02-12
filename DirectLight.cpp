#include "DirectLight.h"
#include <windows.h>
#include <gl/glee.h>
#include "ShaderManager.h"

DirectLight::DirectLight()
{
	m_nColor[0]=1.0f;
	m_nColor[1]=1.0f;
	m_nColor[2]=1.0f;
	m_nAmbient=0.5f;
	m_nDiffuse=0.5f;
	m_nSpecular=1.0f;
	m_nDirection[0]=0;
	m_nDirection[1]=-1.0f;
	m_nDirection[2]=0;
	m_nDirection[3]=0;
	Quaternion quat;
	quat.createQuaternion(0.9f*(2*3.1415f),-1.0f,0.0f,1.0f);
	rotate(quat);
	m_bEnabled=true;
}

void DirectLight::setDirection(float nDirX, float nDirY, float nDirZ)
{
	m_nDirection[0]=nDirX;
	m_nDirection[1]=nDirY;
	m_nDirection[2]=nDirZ;
}

void DirectLight::sendToShader(string sShader)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(sShader);
	glslProgram->use();

	Vector3 vDir = getDirection();

	glslProgram->sendUniform("light.direction", vDir[0],vDir[1],vDir[2]);
	glslProgram->sendUniform("light.color", m_nColor[0],m_nColor[1],m_nColor[2]);
	glslProgram->sendUniform("light.ambient", m_nAmbient);
	glslProgram->sendUniform("light.diffuse", m_nDiffuse);
	if (sShader != "BuildVoxels")
	{
		glslProgram->sendUniform("light.specular", m_nSpecular);
		glslProgram->sendUniform("lightenabled", true);
	}
}

Vector3 DirectLight::getDirection()
{
	Vector4 vDir(m_nDirection[0],m_nDirection[1],m_nDirection[2],m_nDirection[3]);
	Matrix4 m4Rot = this->getRotate().getMatrix();
	Vector4 vFinal = m4Rot*vDir;
	return Vector3(vFinal);
}

Vector3 DirectLight::getUp()
{
	Vector4 vDir(1.0,0,0,0);
	Matrix4 m4Rot = this->getRotate().getMatrix();
	Vector4 vFinal = m4Rot*vDir;
	return Vector3(vFinal);
}

Vector3 DirectLight::getRight()
{
	Vector4 vDir(0,0,1.0,0);
	Matrix4 m4Rot = this->getRotate().getMatrix();
	Vector4 vFinal = m4Rot*vDir;
	return Vector3(vFinal);
}