#include "Matrix.h"

Quaternion::Quaternion() 
{
	s_data = 1.0;
	v_data[0] = 0;
	v_data[1] = 0;
	v_data[2] = 0;
}

Quaternion::Quaternion(float s, float v1, float v2, float v3) 
{
	s_data = s;
	v_data[0] = v1;
	v_data[1] = v2;
	v_data[2] = v3;
}

void Quaternion::createQuaternion(float angle, float axis_x, float axis_y, float axis_z)
{
	Vector3 axis(axis_x,axis_y,axis_z);
	axis.normalize();
	s_data    = cosf( angle/2.0f );
	v_data[0] = axis[0] * sinf( angle/2.0f );
	v_data[1] = axis[1] * sinf( angle/2.0f );
	v_data[2] = axis[2] * sinf( angle/2.0f );
	this->normalize();
}

void Quaternion::createQuaternion(float angle, Vector3 axis)
{
	createQuaternion(angle, axis[0], axis[1], axis[2]);
}

Quaternion Quaternion::normalize()
{
	float magnitude = sqrt((*this)[0]*(*this)[0]+(*this)[1]*(*this)[1]+(*this)[2]*(*this)[2]+(*this)[3]*(*this)[3]);
	(*this)[0] /= magnitude;
	(*this)[1] /= magnitude;
	(*this)[2] /= magnitude;
	(*this)[3] /= magnitude;
	return *this;
}

Matrix4 Quaternion::getMatrix()
{

	float mat[] = { 1-2*(*this)[2]*(*this)[2]-2*(*this)[3]*(*this)[3] , 2*(*this)[1]*(*this)[2]+2*(*this)[0]*(*this)[3]   , 2*(*this)[1]*(*this)[3]-2*(*this)[0]*(*this)[2]   , 0 ,
				  2*(*this)[1]*(*this)[2]-2*(*this)[0]*(*this)[3]   , 1-2*(*this)[1]*(*this)[1]-2*(*this)[3]*(*this)[3] , 2*(*this)[2]*(*this)[3]+2*(*this)[0]*(*this)[1]   , 0 ,
				  2*(*this)[1]*(*this)[3]+2*(*this)[0]*(*this)[2]   , 2*(*this)[2]*(*this)[3]-2*(*this)[0]*(*this)[1]   , 1-2*(*this)[1]*(*this)[1]-2*(*this)[2]*(*this)[2] , 0 ,
				  0												    , 0												    ,  0												, 1 };
	return Matrix4(mat);

}

float &Quaternion::operator [](unsigned int i)
{
	switch (i)
	{
		case 0:
			return s_data;
		case 1:
		case 2:
		case 3:
			return v_data[i-1];
	}
	return s_data;
}

Quaternion	&Quaternion::operator *=(Quaternion &q)
{
	*this=*this*q;
	return *this;
}

bool Quaternion::operator ==(Quaternion &q)
{
	for (int i=0;i<4;i++)
	{
		if ((*this)[i]!=q[i])
		{
			return false;
		}
	}

	return true;
}

bool Quaternion::operator !=(Quaternion &q)
{
	return !(*this==q);
}

Quaternion  Quaternion::operator *(Quaternion &q)
{
	float s1 = s_data;
	float s2 = q[0];
	Vector3 v1 = v_data;
	Vector3 v2 = Vector3(q[1],q[2],q[3]);
	float ret_s = s1*s2 - v1.dot(v2);
	Vector3 ret_v = v2*s1 + v1*s2 + v1.cross(v2);
	
	Quaternion ret;
	ret[0] = ret_s;
	ret[1] = ret_v[0];
	ret[2] = ret_v[1];
	ret[3] = ret_v[2];
	return ret;
}