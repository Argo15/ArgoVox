#include <math.h>
#include "Matrix.h"


Vector3::Vector3()
{
	for (int i=0;i<3;i++)
		data[i]=0;
}

Vector3::Vector3(float v)
{
	for (int i=0;i<3;i++)
		data[i]=v;
}

Vector3::Vector3(float x,float y,float z)
{
	data[0]=x;
	data[1]=y;
	data[2]=z;
}

Vector3::Vector3(Vector3 &v)
{
	*this=v;
}

Vector3::Vector3(Vector4 &v)
{
	*this=v;
}

float &Vector3::operator [](unsigned int i)
{
	return  data[i];
}

Vector3	&Vector3::operator =(Vector3 &v)
{
	for (int i=0;i<3;i++)
	{
		data[i]=v[i];
	}
	return *this;
}

Vector3	&Vector3::operator =(Vector4 &v)
{
	for (int i=0;i<3;i++)
	{
		data[i]=v[i];
	}
	return *this;
}


Vector3	&Vector3::operator +=(Vector3 &v)
{
	*this=*this+v;
	return *this;
}

Vector3	&Vector3::operator -=(Vector3 &v)
{
	*this=*this-v;
	return *this;
}

Vector3	&Vector3::operator /=(Vector3 &v)
{
	*this=*this/v;
	return *this;
}

Vector3	&Vector3::operator *=(Vector3 &v)
{
	*this=*this*v;
	return *this;
}

Vector3	&Vector3::operator *=(float f)
{
	data[0]=data[0]*f;
	data[1]=data[1]*f;
	data[2]=data[2]*f;
	return *this;
}

bool Vector3::operator ==(Vector3 &v)
{
	for (int i=0;i<3;i++)
	{
		if (data[i]!=v[i])
		{
			return false;
		}
	}

	return true;
}

bool Vector3::operator !=(Vector3 &v)
{
	return !(*this==v);
}

Vector3  Vector3::operator +(Vector3 &v)
{
	Vector3 ret = *this;
	for (int i=0;i<3;i++)
	{
		ret[i]=ret[i]+v[i];
	}
	return ret;
}

Vector3  Vector3::operator -(Vector3 &v)
{
	Vector3 ret = *this;
	for (int i=0;i<3;i++)
	{
		ret[i]=ret[i]-v[i];
	}
	return ret;
}

Vector3  Vector3::operator /(Vector3 &v)
{
	Vector3 ret = *this;
	for (int i=0;i<3;i++)
	{
		ret[i]=ret[i]/v[i];
	}
	return ret;
}

Vector3  Vector3::operator *(Vector3 &v)
{
	Vector3 ret = *this;
	for (int i=0;i<3;i++)
	{
		ret[i]=ret[i]*v[i];
	}
	return ret;
}

Vector3  Vector3::operator *(float f)
{
	Vector3 ret = *this;
	for (int i=0;i<3;i++)
	{
		ret[i]=ret[i]*f;
	}
	return ret;
}

void Vector3::set(float x, float y, float z)
{
	data[0]=x;
	data[1]=y;
	data[2]=z;
}

float Vector3::length()
{
	return sqrtf(data[0]*data[0]+data[1]*data[1]+data[2]*data[2]);
}

bool Vector3::isZero()
{
	return (data[0]==0 && data[1]==0 && data[2]==0);

}

Vector3 &Vector3::normalize()
{
	float length = this->length();
	for (int i=0;i<3;i++)
	{
		data[i]/=length;
	}
	return *this;
}

float Vector3::dot(Vector3 &v)
{
	return (data[0]*v[0]+data[1]*v[1]+data[2]*v[2]);
}

Vector3 Vector3::cross(Vector3 &v)
{
	return Vector3(data[1]*v[2]-data[2]*v[1],data[2]*v[0]-data[0]*v[2],data[0]*v[1]-data[1]*v[0]);
}