#ifndef MATRIX_MANAGER_H
#define MATRIX_MANAGER_H

#include "Matrix.h"

enum Matrix4Type {MODELVIEW, PROJECTION};
enum Matrix3Type {NORMAL};

/*
 * Matrix Manager to handle global matricies.
 *
 * Author: wcrane
 * Date: 1/11/2013
 */
class MatrixManager
{
private:
	static MatrixManager *m_pInstance;
	stack<glm::mat4> *m_matrix4;
	stack<glm::mat3> *m_matrix3;

	MatrixManager();
public:
	static MatrixManager *getInstance();

	void putMatrix4(Matrix4Type type, glm::mat4 matrix);
	void putMatrix3(Matrix3Type type, glm::mat3 matrix);
	glm::mat4 getMatrix4(Matrix4Type type);
	glm::mat3 getMatrix3(Matrix3Type type);
	void pushMatrix4(Matrix4Type type, glm::mat4 matrix);
	void pushMatrix3(Matrix3Type type, glm::mat3 matrix);
	void pushMatrix4(Matrix4Type type);
	void pushMatrix3(Matrix3Type type);
	void popMatrix4(Matrix4Type type);
	void popMatrix3(Matrix3Type type);
	void multMatrix4(Matrix4Type type, glm::mat4 matrix);
	void multMatrix3(Matrix3Type type, glm::mat3 matrix);
};

#endif