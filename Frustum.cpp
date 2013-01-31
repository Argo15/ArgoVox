#include "Frustum.h"

#define ANG2RAD 3.14159265358979323846/180.0

Frustum::Frustum()
{

}

void Frustum::getFrustum(Camera *camera, View *view)
{
	Vector3 p = camera->getEyeV();
	Vector3 d = camera->getLookAt()-p;
	d.normalize();
	Vector3 vUp = camera->getUp();
	vUp.normalize();
	Vector3 vRight = d.cross(vUp);
	vRight.normalize();

	float nFov = view->getFOV();
	float nRatio = view->getAspect();

	float nearDist = view->getNear();
	m_nHeightNear = 2 * tan((nFov / 2.0)*(3.1415/180)) * nearDist;
	m_nWidthNear = m_nHeightNear * nRatio;

	float nFarDist = view->getFar();
	m_nHeightFar = 2 * tan((nFov / 2.0)*(3.1415/180)) * nFarDist;
	m_nWidthFar = m_nHeightFar * nRatio;
 
	Vector3 vFarCenter = p + d * nFarDist ; 
	Vector3 vNearCenter = p + d * nearDist;

	m_plane[F_NEAR].vPoint=vNearCenter;
	m_plane[F_FAR].vPoint=vFarCenter;
	m_plane[F_LEFT].vPoint=p;
	m_plane[F_RIGHT].vPoint=p;
	m_plane[F_UP].vPoint=p;
	m_plane[F_DOWN].vPoint=p;

	Vector3 a;

	m_plane[F_NEAR].vNormal=d*-1.0;
	m_plane[F_NEAR].vNormal.normalize();
	m_plane[F_FAR].vNormal=d;
	m_plane[F_FAR].vNormal.normalize();
	a = (vFarCenter - vRight * (m_nWidthFar / 2)) - p;
	a.normalize();
	m_plane[F_LEFT].vNormal=vUp.cross(a);
	m_plane[F_LEFT].vNormal.normalize();
	a = (vFarCenter + vRight * (m_nWidthFar / 2)) - p;
	a.normalize();
	m_plane[F_RIGHT].vNormal=a.cross(vUp);
	m_plane[F_RIGHT].vNormal.normalize();
	a = (vFarCenter + vUp * (m_nHeightFar / 2)) - p;
	a.normalize();
	m_plane[F_UP].vNormal=vRight.cross(a);
	m_plane[F_UP].vNormal.normalize();
	a = (vFarCenter - vUp * (m_nHeightFar / 2)) - p;
	a.normalize();
	m_plane[F_DOWN].vNormal=a.cross(vRight);
	m_plane[F_DOWN].vNormal.normalize();

	for (int i=0; i<6; i++)
	{
		m_plane[i].d=(m_plane[i].vPoint.dot(m_plane[i].vNormal));
	}
}

void Frustum::getOrthoFrustum(Camera *camera, View *view)
{
	Vector3 vLookat = (camera->getLookAt()-camera->getEyeV()).normalize();
	Vector3 vRight = camera->getRight().normalize();
	Vector3 vUp = camera->getUp().normalize();
	Vector3 vCenter = camera->getEyeV()+vLookat*(view->getOrthoFar()-view->getOrthoNear())*0.5;

	m_plane[F_NEAR].vNormal=vLookat*-1.0;
	m_plane[F_FAR].vNormal=vLookat;
	m_plane[F_LEFT].vNormal=vRight*-1.0;
	m_plane[F_RIGHT].vNormal=vRight;
	m_plane[F_UP].vNormal=vUp;
	m_plane[F_DOWN].vNormal=vUp*-1.0;
	
	m_plane[F_NEAR].vPoint=vCenter-vLookat*(view->getOrthoDepth()/2.0);
	m_plane[F_FAR].vPoint=vCenter+vLookat*(view->getOrthoDepth()/2.0);
	m_plane[F_LEFT].vPoint=vCenter-vRight*(view->getOrthoWidth()/2.0);
	m_plane[F_RIGHT].vPoint=vCenter+vRight*(view->getOrthoWidth()/2.0);
	m_plane[F_UP].vPoint=vCenter+vUp*(view->getOrthoHeight()/2.0);
	m_plane[F_DOWN].vPoint=vCenter-vUp*(view->getOrthoHeight()/2.0);

	for (int i=0; i<6; i++)
	{
		m_plane[i].d=(m_plane[i].vPoint.dot(m_plane[i].vNormal));
	}
}

bool Frustum::isInFrustum(Vector3 vCenter, float nRadius)
{
	bool bInFrustum=true;
	for (int i=0; i<6; i++)
	{
		float nDist = m_plane[i].distance(&vCenter);
		if (nDist >= nRadius)
		{
			bInFrustum=false;
		}
	}
	return bInFrustum;
}

void Frustum::calcFarPlane(Camera *camera, View *view)
{
	Vector3 vNearCenter, vFarCenter, vCameraRight, vCameraUp, vCameraLook;

	float ntang = (float)tan(ANG2RAD * view->getFOV() * 0.5) ;
	float nFarHeight = (view->getFar()-0.005) * ntang;
	float nFarWidth = nFarHeight * view->getAspect();

	vCameraRight = camera->getRight();
	vCameraUp = camera->getUp();
	vCameraLook = camera->getLookAt()-camera->getEyeV();
	vCameraLook.normalize();

	vFarCenter = camera->getEyeV()+vCameraLook*(view->getFar()-0.005);

	m_nFarTopLeft = vFarCenter + vCameraUp * nFarHeight - vCameraRight * nFarWidth;
	m_nFarTopRight = vFarCenter + vCameraUp * nFarHeight + vCameraRight * nFarWidth;
	m_nFarBottomLeft = vFarCenter - vCameraUp * nFarHeight - vCameraRight * nFarWidth;
	m_nFarBottomRight = vFarCenter - vCameraUp * nFarHeight + vCameraRight * nFarWidth;
}

float Frustum::getHeightNear() 
{
	return m_nHeightNear;
}

float Frustum::getWidthNear() 
{
	return m_nWidthNear;
}

float Frustum::getHeightFar() 
{
	return m_nHeightFar;
}

float Frustum::getWidthFar() 
{
	return m_nWidthFar;
}