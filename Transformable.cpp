#include "Transformable.h"

Transformable::Transformable()
{
	for (int i=0;i<3;i++)
	{
		m_nTranslations[i]=0;
		m_nScale[i]=1.0;
	}
	m_qRotation = Quaternion();
}

void Transformable::transform()
{
	glMatrixMode(GL_MODELVIEW);
	glTranslatef(m_nTranslations[0],m_nTranslations[1],m_nTranslations[2]);
	m_qRotation.getMatrix().multiplyToCurrent();
	glScalef(m_nScale[0],m_nScale[1],m_nScale[2]);
}

glm::mat4 Transformable::transformToMatrix(glm::mat4 m4Matrix)
{
	glm::mat4 m4Mat = glm::mat4(1.0f);
	m4Mat = glm::translate(m4Mat, glm::vec3(m_nTranslations[0],m_nTranslations[1],m_nTranslations[2]));
	glm::quat rot = glm::quat(m_qRotation[0],m_qRotation[1],m_qRotation[2],m_qRotation[3]);
	m4Mat = m4Mat * glm::mat4_cast(rot);
	m4Mat = glm::scale(m4Mat, glm::vec3(m_nScale[0],m_nScale[1],m_nScale[2]));
	return m4Matrix * m4Mat;
}

glm::mat3 Transformable::transformToMatrix(glm::mat3 m3Matrix)
{
	glm::mat3 m4Mat = glm::mat3(1.0f);
	glm::quat rot = glm::quat(m_qRotation[0],m_qRotation[1],m_qRotation[2],m_qRotation[3]);
	m4Mat = m4Mat * glm::mat3_cast(rot);
	return m3Matrix * m4Mat;
}

void Transformable::translate(float nTransX, float nTransY, float nTransZ) 
{
	m_nTranslations[0] += nTransX;
	m_nTranslations[1] += nTransY;
	m_nTranslations[2] += nTransZ;
}

void Transformable::rotate(Quaternion quat) 
{
	m_qRotation = quat * m_qRotation;
}

void Transformable::scale(float nScaleX, float nScaleY, float nScaleZ) 
{
	m_nScale[0] += nScaleX;
	m_nScale[1] += nScaleY;
	m_nScale[2] += nScaleZ;
}

void Transformable::setTranslate(float nTransX, float nTransY, float nTransZ) 
{
	m_nTranslations[0] = nTransX;
	m_nTranslations[1] = nTransY;
	m_nTranslations[2] = nTransZ;
}

void Transformable::setRotate(Quaternion quat) 
{
	m_qRotation = quat;
}

void Transformable::setScale(float nScaleX, float nScaleY, float nScaleZ) 
{
	m_nScale[0] = nScaleX;
	m_nScale[1] = nScaleY;
	m_nScale[2] = nScaleZ;
}

float *Transformable::getTranslate() 
{
	float *pTranslate = m_nTranslations; 
	return pTranslate;
}

Quaternion Transformable::getRotate() 
{
	return m_qRotation;
}

float *Transformable::getScale() 
{
	float *pnScale;
	pnScale=m_nScale; 
	return pnScale;
}

Vector3 Transformable::getTranslateV() 
{
	return Vector3(m_nTranslations[0], m_nTranslations[1], m_nTranslations[2]);
}