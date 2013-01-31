#include "Matrix.h"
#include <iostream>

Matrix4::Matrix4()
{
	this->identity();
}

Matrix4::Matrix4(float v)
{
	for (int i=0;i<16;i++)
		data[i]=v;
}

Matrix4::Matrix4(float *v)
{
	for (int i=0;i<16;i++)
		data[i]=v[i];
}	

Matrix4::Matrix4(Matrix4 &m)
{
	*this=m;
}

Matrix4::Matrix4(Matrix3 &m)
{
	*this=m;
}

float Matrix4::operator [](unsigned int i)
{
	return data[i];
}

Matrix4& Matrix4::operator =(Matrix4 &m)
{
	for (int i=0;i<16;i++)
		data[i]=m[i];
	return *this;
}

Matrix4& Matrix4::operator =(Matrix3 &m)
{
	this->identity();
	data[0]=m[0];
	data[1]=m[1];
	data[2]=m[2];
	data[4]=m[3];
	data[5]=m[4];
	data[6]=m[5];
	data[8]=m[6];
	data[9]=m[7];
	data[10]=m[8];
	return *this;
}


Matrix4	&Matrix4::operator +=(Matrix4 &m)
{
	*this=*this+m;
	return *this;
}

Matrix4	&Matrix4::operator -= (Matrix4 &m)
{
	*this=*this-m;
	return *this;
}

Matrix4	&Matrix4::operator /= (Matrix4 &m)
{
	*this=*this/m;
	return *this;
}

Matrix4	&Matrix4::operator *= (Matrix4 &m)
{
	*this=*this*m;
	return *this;
}

Matrix4	&Matrix4::operator *= (float f)
{
	for (int i=0;i<16;i++)
		data[i]=data[i]*f;
	return *this;
}

bool Matrix4::operator ==(Matrix4 &m)
{
	for (int i=0;i<16;i++)
		if (data[i]!=m.data[i])
			return false;

	return true;
}

bool Matrix4::operator !=(Matrix4 &m)
{
	return !(*this==m);
}

Matrix4 Matrix4::operator +(Matrix4 &m)
{
	for (int i=0;i<16;i++)
		data[i]=data[i]+m.data[i];
	return *this;
}

Matrix4 Matrix4::operator -(Matrix4 &m)
{
	for (int i=0;i<16;i++)
		data[i]=data[i]+m.data[i];
	return *this;
}

Matrix4 Matrix4::operator /(Matrix4 &m)
{
	Matrix4 inv = this->inverse();
	Matrix4 ret = m*inv;
	return ret;
}

Matrix4 Matrix4::operator *(Matrix4 &m)
{
	Vector4 row[4];
	Vector4 col[4];

	row[0].set(data[0],data[4],data[8],data[12]);
	row[1].set(data[1],data[5],data[9],data[13]);
	row[2].set(data[2],data[6],data[10],data[14]);
	row[3].set(data[3],data[7],data[11],data[15]);
	col[0].set(m.data[0],m.data[1],m.data[2],m.data[3]);
	col[1].set(m.data[4],m.data[5],m.data[6],m.data[7]);
	col[2].set(m.data[8],m.data[9],m.data[10],m.data[11]);
	col[3].set(m.data[12],m.data[13],m.data[14],m.data[15]);
	data[0]=row[0].dot(col[0]);
	data[1]=row[1].dot(col[0]);
	data[2]=row[2].dot(col[0]);
	data[3]=row[3].dot(col[0]);
	data[4]=row[0].dot(col[1]);
	data[5]=row[1].dot(col[1]);
	data[6]=row[2].dot(col[1]);
	data[7]=row[3].dot(col[1]);
	data[8]=row[0].dot(col[2]);
	data[9]=row[1].dot(col[2]);
	data[10]=row[2].dot(col[2]);
	data[11]=row[3].dot(col[2]);
	data[12]=row[0].dot(col[3]);
	data[13]=row[1].dot(col[3]);
	data[14]=row[2].dot(col[3]);
	data[15]=row[3].dot(col[3]);
	return *this;
}

Vector4 Matrix4::operator *(Vector4 &v)
{
	Vector4 ret;
	Vector4 row[4];

	row[0].set(data[0],data[4],data[8],data[12]);
	row[1].set(data[1],data[5],data[9],data[13]);
	row[2].set(data[2],data[6],data[10],data[14]);
	row[3].set(data[3],data[7],data[11],data[15]);

	ret.set(row[0].dot(v),row[1].dot(v),row[2].dot(v),row[3].dot(v));
	return ret;
}

Matrix4 &Matrix4::identity()
{
	for(int i=0;i<16;i++)
		data[i]=0;
	data[0]=1.0;
	data[5]=1.0;
	data[10]=1.0;
	data[15]=1.0;
	return *this;
}

Matrix4 &Matrix4::transpose()
{
	Matrix4 m;
	m.data[0]=data[0];
	m.data[1]=data[4];
	m.data[2]=data[8];
	m.data[3]=data[12];
	m.data[4]=data[1];
	m.data[5]=data[5];
	m.data[6]=data[9];
	m.data[7]=data[13];
	m.data[8]=data[2];
	m.data[9]=data[6];
	m.data[10]=data[10];
	m.data[11]=data[14];
	m.data[12]=data[3];
	m.data[13]=data[7];
	m.data[14]=data[11];
	m.data[15]=data[15];
	*this=m;
	return *this;
}

Matrix4 Matrix4::inverse()
{
	// using RReF
	/*| 0  4  8   12 |
	  | 1  5  9   13 |
	  | 2  6  10  14 |
	  | 3  7  11  15 |*/

	Matrix4 inv;
	Matrix4 rref = *this;
	float divisor;
	float multiplier;
	


	divisor=rref.data[0];
	if (divisor==0){
		std::cout << "Cannot invert matrix 4x4" << std::endl;
		return inv.identity();
	}
	rref.data[0] /= divisor;
	rref.data[4] /= divisor;
	rref.data[8] /= divisor;
	rref.data[12] /= divisor;
	inv.data[0] /= divisor;
	inv.data[4] /= divisor;
	inv.data[8] /= divisor;
	inv.data[12] /= divisor;

	multiplier=rref.data[1];
	rref.data[1] -= multiplier*rref.data[0];
	rref.data[5] -= multiplier*rref.data[4];
	rref.data[9] -= multiplier*rref.data[8];
	rref.data[13] -= multiplier*rref.data[12];
	inv.data[1] -= multiplier*inv.data[0];
	inv.data[5] -= multiplier*inv.data[4];
	inv.data[9] -= multiplier*inv.data[8];
	inv.data[13] -= multiplier*inv.data[12];

	multiplier=rref.data[2];
	rref.data[2] -= multiplier*rref.data[0];
	rref.data[6] -= multiplier*rref.data[4];
	rref.data[10] -= multiplier*rref.data[8];
	rref.data[14] -= multiplier*rref.data[12];
	inv.data[2] -= multiplier*inv.data[0];
	inv.data[6] -= multiplier*inv.data[4];
	inv.data[10] -= multiplier*inv.data[8];
	inv.data[14] -= multiplier*inv.data[12];

	multiplier=rref.data[3];
	rref.data[3] -= multiplier*rref.data[0];
	rref.data[7] -= multiplier*rref.data[4];
	rref.data[11] -= multiplier*rref.data[8];
	rref.data[15] -= multiplier*rref.data[12];
	inv.data[3] -= multiplier*inv.data[0];
	inv.data[7] -= multiplier*inv.data[4];
	inv.data[11] -= multiplier*inv.data[8];
	inv.data[15] -= multiplier*inv.data[12];



	divisor=rref.data[5];
	if (divisor==0){
		std::cout << "Cannot invert matrix 4x4" << std::endl;
		return inv.identity();
	}
	rref.data[5] /= divisor;
	rref.data[9] /= divisor;
	rref.data[13] /= divisor;
	inv.data[1] /= divisor;
	inv.data[5] /= divisor;
	inv.data[9] /= divisor;
	inv.data[13] /= divisor;

	multiplier=rref.data[4];
	rref.data[4] -= multiplier*rref.data[5];
	rref.data[8] -= multiplier*rref.data[9];
	rref.data[12] -= multiplier*rref.data[13];
	inv.data[0] -= multiplier*inv.data[1];
	inv.data[4] -= multiplier*inv.data[5];
	inv.data[8] -= multiplier*inv.data[9];
	inv.data[12] -= multiplier*inv.data[13];

	multiplier=rref.data[6];
	rref.data[6] -= multiplier*rref.data[5];
	rref.data[10] -= multiplier*rref.data[9];
	rref.data[14] -= multiplier*rref.data[13];
	inv.data[2] -= multiplier*inv.data[1];
	inv.data[6] -= multiplier*inv.data[5];
	inv.data[10] -= multiplier*inv.data[9];
	inv.data[14] -= multiplier*inv.data[13];

	multiplier=rref.data[7];
	rref.data[7] -= multiplier*rref.data[5];
	rref.data[11] -= multiplier*rref.data[9];
	rref.data[15] -= multiplier*rref.data[13];
	inv.data[3] -= multiplier*inv.data[1];
	inv.data[7] -= multiplier*inv.data[5];
	inv.data[11] -= multiplier*inv.data[9];
	inv.data[15] -= multiplier*inv.data[13];



	divisor=rref.data[10];
	if (divisor==0){
		std::cout << "Cannot invert matrix 4x4" << std::endl;
		return inv.identity();
	}
	rref.data[10] /= divisor;
	rref.data[14] /= divisor;
	inv.data[2] /= divisor;
	inv.data[6] /= divisor;
	inv.data[10] /= divisor;
	inv.data[14] /= divisor;

	multiplier=rref.data[8];
	rref.data[8] -= multiplier*rref.data[10];
	rref.data[12] -= multiplier*rref.data[14];
	inv.data[0] -= multiplier*inv.data[2];
	inv.data[4] -= multiplier*inv.data[6];
	inv.data[8] -= multiplier*inv.data[10];
	inv.data[12] -= multiplier*inv.data[14];

	multiplier=rref.data[9];
	rref.data[9] -= multiplier*rref.data[10];
	rref.data[13] -= multiplier*rref.data[14];
	inv.data[1] -= multiplier*inv.data[2];
	inv.data[5] -= multiplier*inv.data[6];
	inv.data[9] -= multiplier*inv.data[10];
	inv.data[13] -= multiplier*inv.data[14];

	multiplier=rref.data[11];
	rref.data[11] -= multiplier*rref.data[10];
	rref.data[15] -= multiplier*rref.data[14];
	inv.data[3] -= multiplier*inv.data[2];
	inv.data[7] -= multiplier*inv.data[6];
	inv.data[11] -= multiplier*inv.data[10];
	inv.data[15] -= multiplier*inv.data[14];



	divisor=rref.data[15];
	if (divisor==0){
		std::cout << "Cannot invert matrix 4x4" << std::endl;
		return inv.identity();
	}
	rref.data[15] /= divisor;
	inv.data[3] /= divisor;
	inv.data[7] /= divisor;
	inv.data[11] /= divisor;
	inv.data[15] /= divisor;

	multiplier=rref.data[12];
	rref.data[12] -= multiplier*rref.data[15];
	inv.data[0] -= multiplier*inv.data[3];
	inv.data[4] -= multiplier*inv.data[7];
	inv.data[8] -= multiplier*inv.data[11];
	inv.data[12] -= multiplier*inv.data[15];

	multiplier=rref.data[13];
	rref.data[13] -= multiplier*rref.data[15];
	inv.data[1] -= multiplier*inv.data[3];
	inv.data[5] -= multiplier*inv.data[7];
	inv.data[9] -= multiplier*inv.data[11];
	inv.data[13] -= multiplier*inv.data[15];

	multiplier=rref.data[14];
	rref.data[14] -= multiplier*rref.data[15];
	inv.data[2] -= multiplier*inv.data[3];
	inv.data[6] -= multiplier*inv.data[7];
	inv.data[10] -= multiplier*inv.data[11];
	inv.data[14] -= multiplier*inv.data[15];

	return inv;
}

Matrix4 &Matrix4::setAsModelViewProjection()
{
	Matrix4 modelView;
	Matrix4 projection;
	modelView.setAsModelViewMatrix();
	projection.setAsProjectionMatrix();
	*this = projection*modelView;
	return *this;
}

Matrix4 &Matrix4::setAsModelViewMatrix()
{
	float curMV[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, curMV); 
	Matrix4 modelView(curMV);
	*this=modelView;
	return *this;
}

Matrix4 &Matrix4::setAsProjectionMatrix()
{
	float curProj[16];
	glGetFloatv(GL_PROJECTION_MATRIX, curProj); 
	Matrix4 proj(curProj);
	*this=proj;
	return *this;
}

void Matrix4::multiplyToCurrent()
{
	glMultMatrixf(data);
}
