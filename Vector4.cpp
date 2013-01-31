#include <math.h>
#include "Matrix.h"

Vector4::Vector4()
{
	for (int i=0;i<4;i++)
	{
		data[i]=0;
	}
}

Vector4::Vector4(float v)
{
	for (int i=0;i<4;i++)
	{
		data[i]=v;
	}
}

Vector4::Vector4(float x,float y,float z,float w)
{
	data[0]=x;
	data[1]=y;
	data[2]=z;
	data[3]=w;
}

Vector4::Vector4(Vector4 &v)
{
	*this=v;
}

Vector4::Vector4(Vector3 &v)
{
	*this=v;
	data[3]=1;
}

float &Vector4::operator [](unsigned int i)
{
	return data[i];
}

Vector4	&Vector4::operator =(Vector4 &v)
{
	for (int i=0;i<4;i++)
	{
		data[i]=v[i];
	}
	return *this;
}

Vector4	&Vector4::operator =(Vector3 &v)
{
	for (int i=0;i<3;i++)
	{
		data[i]=v[i];
	}
	data[3]=0;
	return *this;
}


Vector4	&Vector4::operator +=(Vector4 &v)
{
	*this=*this+v;
	return *this;
}

Vector4	&Vector4::operator -=(Vector4 &v)
{
	*this=*this-v;
	return *this;
}

Vector4	&Vector4::operator /=(Vector4 &v)
{
	*this=*this/v;
	return *this;
}

Vector4	&Vector4::operator *=(Vector4 &v)
{
	*this=*this*v;
	return *this;
}

Vector4	&Vector4::operator *=(float f)
{
	data[0]=data[0]*f;
	data[1]=data[1]*f;
	data[2]=data[2]*f;
	data[3]=data[3]*f;
	return *this;
}

bool Vector4::operator ==(Vector4 &v)
{
	for (int i=0;i<4;i++)
	{
		if (data[i]!=v[i])
		{
			return false;
		}
	}

	return true;
}

bool Vector4::operator !=(Vector4 &v)
{
	return !(*this==v);
}

Vector4  Vector4::operator +(Vector4 &v)
{
	Vector4 ret = *this;
	for (int i=0;i<4;i++)
	{
		ret[i]=ret[i]+v[i];
	}
	return ret;
}

Vector4  Vector4::operator -(Vector4 &v)
{
	Vector4 ret = *this;
	for (int i=0;i<4;i++)
	{
		ret[i]=ret[i]-v[i];
	}
	return ret;
}

Vector4  Vector4::operator /(Vector4 &v)
{
	Vector4 ret = *this;
	for (int i=0;i<4;i++)
	{
		ret[i]=ret[i]/v[i];
	}
	return ret;
}

Vector4  Vector4::operator *(Vector4 &v)
{
	Vector4 ret = *this;
	for (int i=0;i<4;i++)
	{
		ret[i]=ret[i]*v[i];
	}
	return ret;
}

Vector4  Vector4::operator *(float f)
{
	Vector4 ret = *this;
	for (int i=0;i<4;i++)
	{
		ret[i]=ret[i]*f;
	}
	return ret;
}

void Vector4::set(float x, float y, float z, float w)
{
	data[0]=x;
	data[1]=y;
	data[2]=z;
	data[3]=w;
}

float Vector4::length()
{
	return sqrtf(data[0]*data[0]+data[1]*data[1]+data[2]*data[2]+data[3]*data[3]);
}

bool Vector4::isZero()
{
	return (data[0]==0 && data[1]==0 && data[2]==0 && data[3]==0);

}

Vector4 &Vector4::normalize()
{
	float length = this->length();
	for (int i=0;i<4;i++)
	{
		data[i]/=length;
	}
	return *this;
}

float Vector4::dot(Vector4 &v)
{
	return (data[0]*v[0]+data[1]*v[1]+data[2]*v[2]+data[3]*v[3]);
}

void Vector4::rotate(float angle, float x, float y, float z)
{
	float c=cosf(angle);
	float s=sinf(angle);
	float v[] = { x*x*(1-c)+c   , x*y*(1-c)-z*s , x*z*(1-c)+y*s , 0,
				  y*x*(1-c)+z*s , y*y*(1-c)+c   , y*z*(1-c)+x*s , 0,
				  x*z*(1-c)+y*s , y*z*(1-c)-x*s , z*z*(1-c)+c   , 0,
				  0				, 0				, 0				, 1 };
	Matrix4 m(v);
	*this=m*(*this);
}