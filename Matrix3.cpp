#include "Matrix.h"


Matrix3::Matrix3()
{
	this->identity();
}

Matrix3::Matrix3(float v)
{
	for (int i=0;i<9;i++)
		data[i]=v;
}

Matrix3::Matrix3(float *v)
{
	for (int i=0;i<9;i++)
		data[i]=v[i];
}	

Matrix3::Matrix3(Matrix3 &m)
{
	*this=m;
}

Matrix3::Matrix3(Matrix4 &m)
{
	*this=m;
}

float Matrix3::operator [](unsigned int i)
{
	return data[i];
}

Matrix3& Matrix3::operator =(Matrix3 &m)
{
	for (int i=0;i<9;i++)
		data[i]=m[i];
	return *this;
}

Matrix3& Matrix3::operator =(Matrix4 &m)
{
	this->identity();
	data[0]=m[0];
	data[1]=m[1];
	data[2]=m[2];
	data[3]=m[4];
	data[4]=m[5];
	data[5]=m[6];
	data[6]=m[8];
	data[7]=m[9];
	data[8]=m[10];
	return *this;
}


Matrix3	&Matrix3::operator +=(const Matrix3 &m)
{
	*this=*this+m;
	return *this;
}

Matrix3	&Matrix3::operator -= (const Matrix3 &m)
{
	*this=*this-m;
	return *this;
}

Matrix3	&Matrix3::operator /= (const Matrix3 &m)
{
	*this=*this/m;
	return *this;
}

Matrix3	&Matrix3::operator *= (const Matrix3 &m)
{
	*this=*this*m;
	return *this;
}

Matrix3	&Matrix3::operator *= (float f)
{
	for (int i=0;i<9;i++)
		data[i]=data[i]*f;
	return *this;
}

bool Matrix3::operator ==(const Matrix3 &m)
{
	for (int i=0;i<9;i++)
		if (data[i]!=m.data[i])
			return false;

	return true;
}

bool Matrix3::operator !=(const Matrix3 &m)
{
	return !(*this==m);
}

Matrix3 Matrix3::operator +(const Matrix3 &m)
{
	for (int i=0;i<9;i++)
		data[i]=data[i]+m.data[i];
	return *this;
}

Matrix3 Matrix3::operator -(const Matrix3 &m)
{
	for (int i=0;i<9;i++)
		data[i]=data[i]+m.data[i];
	return *this;
}

Matrix3 Matrix3::operator /(const Matrix3 &m)
{
	// not implemented
	return *this;
}

Matrix3 Matrix3::operator *(const Matrix3 &m)
{
	Vector3 row[3];
	Vector3 col[3];

	row[0].set(data[0],data[3],data[6]);
	row[1].set(data[1],data[4],data[7]);
	row[2].set(data[2],data[5],data[8]);
	col[0].set(m.data[0],m.data[1],m.data[2]);
	col[1].set(m.data[3],m.data[4],m.data[5]);
	col[2].set(m.data[6],m.data[7],m.data[8]);
	data[0]=row[0].dot(col[0]);
	data[1]=row[1].dot(col[0]);
	data[2]=row[2].dot(col[0]);
	data[3]=row[0].dot(col[1]);
	data[4]=row[1].dot(col[1]);
	data[5]=row[2].dot(col[1]);
	data[6]=row[0].dot(col[2]);
	data[7]=row[1].dot(col[2]);
	data[8]=row[2].dot(col[2]);
	return *this;
}

Vector3 Matrix3::operator *(Vector3 &v)
{
	Vector3 ret;
	Vector3 row[3];

	row[0].set(data[0],data[3],data[6]);
	row[1].set(data[1],data[4],data[7]);
	row[2].set(data[2],data[5],data[8]);

	ret.set(row[0].dot(v),row[1].dot(v),row[2].dot(v));
	return ret;
}

Matrix3 &Matrix3::identity()
{
	for(int i=0;i<9;i++)
		data[i]=0;
	data[0]=1.0;
	data[4]=1.0;
	data[8]=1.0;
	return *this;
}

Matrix3 &Matrix3::transpose()
{
	Matrix3 m;
	m.data[0]=data[0];
	m.data[1]=data[3];
	m.data[2]=data[6];
	m.data[3]=data[1];
	m.data[4]=data[4];
	m.data[5]=data[7];
	m.data[6]=data[2];
	m.data[7]=data[5];
	m.data[8]=data[8];
	*this=m;
	return *this;
}