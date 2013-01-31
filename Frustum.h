#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "Camera.h"
#include "View.h"
#include "Matrix.h"

#define F_NEAR	0
#define F_FAR	1
#define F_LEFT	2
#define F_RIGHT	3
#define F_UP	4
#define F_DOWN	5

struct FrustumPlane
{
	Vector3 vPoint;
	Vector3 vNormal;
	float d;

	float distance(Vector3 *p)
	{
		return (*p).dot(vNormal)-d;
	}
};

class Frustum
{
private:
	FrustumPlane m_plane[6];
	float m_nHeightNear;
	float m_nWidthNear;
	float m_nHeightFar;
	float m_nWidthFar;
	Vector3 m_nFarTopLeft;
	Vector3 m_nFarTopRight;
	Vector3 m_nFarBottomLeft;
	Vector3 m_nFarBottomRight;

public:
	Frustum();

	void getFrustum(Camera *camera, View *view);
	void getOrthoFrustum(Camera *camera, View *view);
	void calcFarPlane(Camera *camera, View *view);
	bool isInFrustum(Vector3 vCenter, float nRadius);

	float getHeightNear();
	float getWidthNear();
	float getHeightFar();
	float getWidthFar();
};

#endif