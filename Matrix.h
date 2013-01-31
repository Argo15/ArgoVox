#ifndef MATRIX_H
#define MATRIX_H

#include <windows.h>
#undef __glext_h_
#undef __glxext_h_
#undef __gl_h_
#include <GL/GLee.h>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <stack>
using namespace std;

class Matrix3;
class Matrix4;
class Vector3;
class Vector4;

class Matrix4
{
private:
	float data[16];	  /*| 0  4  8   12 |
						| 1  5  9   13 |
						| 2  6  10  14 |
						| 3  7  11  15 |*/
public:
	// constructors
	Matrix4();
	Matrix4(float v);
	Matrix4(float *v);
	Matrix4(Matrix4 &m);
	Matrix4(Matrix3 &m);

	// operators
	float		 operator [] (unsigned int i);
	Matrix4	&operator =  (Matrix4 &m);
	Matrix4	&operator =  (Matrix3 &m);
	Matrix4	&operator += (Matrix4 &m);
	Matrix4	&operator -= (Matrix4 &m);
	Matrix4	&operator /= (Matrix4 &m);
	Matrix4	&operator *= (Matrix4 &m);
	Matrix4	&operator *= (float f);
	bool		 operator == (Matrix4 &m);
	bool		 operator != (Matrix4 &m);
	Matrix4	 operator +	 (Matrix4 &m);
	Matrix4	 operator -	 (Matrix4 &m);
	Matrix4	 operator /	 (Matrix4 &m);
	Matrix4	 operator *	 (Matrix4 &m);
	Vector4  operator *  (Vector4 &v);

	// methods
	Matrix4 &identity();
	Matrix4 &transpose();
	Matrix4 inverse();
	Matrix4 &setAsModelViewProjection();
	Matrix4 &setAsModelViewMatrix();
	Matrix4 &setAsProjectionMatrix();
	void multiplyToCurrent();
};


class Matrix3
{
private:
	float data[9];	  /*| 0  3  6 |
						| 1  4  7 |
						| 2  5  8 |*/
public:
	// constructors
	Matrix3();
	Matrix3(float v);
	Matrix3(float *v);
	Matrix3(Matrix3 &m);
	Matrix3(Matrix4 &m);

	// operators
	float		 operator [] (unsigned int i);
	Matrix3	&operator =  (Matrix3 &m);
	Matrix3	&operator =  (Matrix4 &m);
	Matrix3	&operator += (const Matrix3 &m);
	Matrix3	&operator -= (const Matrix3 &m);
	Matrix3	&operator /= (const Matrix3 &m);
	Matrix3	&operator *= (const Matrix3 &m);
	Matrix3	&operator *= (float f);
	bool		 operator == (const Matrix3 &m);
	bool		 operator != (const Matrix3 &m);
	Matrix3  operator +	 (const Matrix3 &m);
	Matrix3  operator -	 (const Matrix3 &m);
	Matrix3  operator /	 (const Matrix3 &m);
	Matrix3  operator *	 (const Matrix3 &m);
	Vector3  operator *  (Vector3 &v);

	// methods
	Matrix3 &identity();
	Matrix3 &transpose();
};


class Vector4
{
private:
	float data[4];

public:
	// constructors
	Vector4();
	Vector4(float v);
	Vector4(float x,float y,float z,float w);
	Vector4(Vector4 &v);
	Vector4(Vector3 &v);

	// operators
	float		&operator [] (unsigned int i);
	Vector4	&operator =  (Vector4 &v);
	Vector4	&operator =  (Vector3 &v);
	Vector4	&operator += (Vector4 &v);
	Vector4	&operator -= (Vector4 &v);
	Vector4	&operator /= (Vector4 &v);
	Vector4	&operator *= (Vector4 &v);
	Vector4	&operator *= (float f);
	bool		 operator == (Vector4 &v);
	bool		 operator != (Vector4 &v);
	Vector4  operator +	 (Vector4 &v);
	Vector4  operator -	 (Vector4 &v);
	Vector4  operator /	 (Vector4 &v);
	Vector4  operator *	 (Vector4 &v);
	Vector4  operator *	 (float f);

	// methods
	void set(float x, float y, float z, float w);
	float length();
	bool isZero();
	Vector4 &normalize();
	float dot(Vector4 &v);
	void rotate(float angle, float x, float y, float z);
};


class Vector3
{
private:
	float data[3];

public:
	// constructors
	Vector3();
	Vector3(float v);
	Vector3(float x,float y,float z);
	Vector3(Vector3 &v);
	Vector3(Vector4 &v);

	// operators
	float		&operator [] (unsigned int i);
	Vector3	&operator =  (Vector3 &v);
	Vector3	&operator =  (Vector4 &v);
	Vector3	&operator += (Vector3 &v);
	Vector3	&operator -= (Vector3 &v);
	Vector3	&operator /= (Vector3 &v);
	Vector3	&operator *= (Vector3 &v);
	Vector3	&operator *= (float f);
	bool		 operator == (Vector3 &v);
	bool		 operator != (Vector3 &v);
	Vector3  operator +	 (Vector3 &v);
	Vector3  operator -	 (Vector3 &v);
	Vector3  operator /	 (Vector3 &v);
	Vector3  operator *	 (Vector3 &v);
	Vector3  operator *	 (float f);

	// methods
	void set(float x, float y, float z);
	float length();
	bool isZero();
	Vector3 &normalize();
	float dot(Vector3 &v);
	Vector3 cross(Vector3 &v);
};


class Quaternion
{
private:
	Vector3 v_data;
	float		s_data;

public:
	Quaternion();
	Quaternion(float s, float v1, float v2, float v3);
	void createQuaternion(float angle, float axis_x, float axis_y, float axis_z);
	void createQuaternion(float angle, Vector3 axis);
	Quaternion normalize();
	Matrix4 getMatrix();

	float		&operator [] (unsigned int i);
	Quaternion	&operator *= (Quaternion &q);
	bool		operator ==  (Quaternion &q);
	bool		operator !=  (Quaternion &q);
	Quaternion  operator *	 (Quaternion &q);
};

#endif