#include "MatrixManager.h"

static int N_NUM_MATRIX4 = 2;
static int N_NUM_MATRIX3 = 1;

MatrixManager* MatrixManager::m_pInstance = 0;

MatrixManager::MatrixManager()
{
	m_matrix4 = new stack<glm::mat4>[N_NUM_MATRIX4];
	m_matrix3 = new stack<glm::mat3>[N_NUM_MATRIX3];
	pushMatrix4(MODELVIEW, glm::mat4(1.0f)); 
	pushMatrix4(PROJECTION, glm::mat4(1.0f)); 
	pushMatrix3(NORMAL, glm::mat3(1.0f)); 
}

MatrixManager *MatrixManager::getInstance()
{
	if (m_pInstance == 0)
	{
		m_pInstance = new MatrixManager();
	}
	return m_pInstance;
}

void MatrixManager::putMatrix4(Matrix4Type type, glm::mat4 matrix)
{
	m_matrix4[(int)type].top() = matrix;
}
	
void MatrixManager::putMatrix3(Matrix3Type type, glm::mat3 matrix)
{
	m_matrix3[(int)type].top() = matrix;
}
	
glm::mat4 MatrixManager::getMatrix4(Matrix4Type type)
{
	return m_matrix4[(int)type].top();
}
	
glm::mat3 MatrixManager::getMatrix3(Matrix3Type type)
{
	return m_matrix3[(int)type].top();
}
	
void MatrixManager::pushMatrix4(Matrix4Type type, glm::mat4 matrix)
{
	m_matrix4[(int)type].push(matrix);
}
	
void MatrixManager::pushMatrix3(Matrix3Type type, glm::mat3 matrix)
{
	m_matrix3[(int)type].push(matrix);
}

void MatrixManager::pushMatrix4(Matrix4Type type)
{
	m_matrix4[(int)type].push(m_matrix4[(int)type].top());
}

void MatrixManager::pushMatrix3(Matrix3Type type)
{
	m_matrix3[(int)type].push(m_matrix3[(int)type].top());
}
	
void MatrixManager::popMatrix4(Matrix4Type type)
{
	m_matrix4[(int)type].pop();
}
	
void MatrixManager::popMatrix3(Matrix3Type type)
{
	m_matrix3[(int)type].pop();
}

void MatrixManager::multMatrix4(Matrix4Type type, glm::mat4 matrix)
{
	m_matrix4[(int)type].top() *= matrix;
}

void MatrixManager::multMatrix3(Matrix3Type type, glm::mat3 matrix)
{
	m_matrix3[(int)type].top() *= matrix;
}