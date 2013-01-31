#include "StaticModel.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

void StaticModel::draw()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glEnableVertexAttribArray(4);

	glBindBufferARB(GL_ARRAY_BUFFER,m_nBufferID);
	glVertexAttribPointerARB(0,3,GL_FLOAT,GL_FALSE,0,0);
	glVertexAttribPointerARB(1,2,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(m_nNumTriangles*9*sizeof(GLfloat)));
	glVertexAttribPointerARB(2,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(m_nNumTriangles*15*sizeof(GLfloat)));
	glVertexAttribPointerARB(3,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(m_nNumTriangles*24*sizeof(GLfloat)));
	glVertexAttribPointerARB(4,3,GL_FLOAT,GL_FALSE,0,BUFFER_OFFSET(m_nNumTriangles*33*sizeof(GLfloat)));
	glDrawArrays(GL_TRIANGLES, 0, m_nNumTriangles*3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDisableVertexAttribArray(4);
}

void StaticModel::drawGeometry()
{
	glEnableVertexAttribArray(0);

	glBindBufferARB(GL_ARRAY_BUFFER,m_nBufferID);
	glVertexAttribPointerARB(0,3,GL_FLOAT,GL_FALSE,0,0);
	glDrawArrays(GL_TRIANGLES, 0, m_nNumTriangles*3);

	glDisableVertexAttribArray(0);
}

void StaticModel::remove() 
{ 
	glDeleteBuffersARB(1, &m_nBufferID); 
}