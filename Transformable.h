#ifndef TRANSFORMABLE_H
#define TRANSFORMABLE_H

#include "Matrix.h"

class Transformable {
protected:
	float m_nTranslations[3];
	Quaternion m_qRotation;
	float m_nScale[3];

public:
	Transformable();

	void transform();
	glm::mat4 transformToMatrix(glm::mat4 m4Matrix);
	glm::mat3 transformToMatrix(glm::mat3 m3matrix);

	void translate(float nTransX, float nTransY, float nTransZ);
	void rotate(Quaternion quat);
	void scale(float nScaleX, float nScaleY, float nScaleZ);
	void setTranslate(float nTransX, float nTransY, float nTransZ);
	void setRotate(Quaternion quat);
	void setScale(float nScaleX, float nScaleY, float nScaleZ);
	float *getTranslate();
	Quaternion getRotate();
	float *getScale();
	Vector3 getTranslateV();
};

#endif