#include "AObjModel.h"

bool AObjModel::load(string sFileName)
{
	ifstream file(sFileName.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{
		float nVersion=0;
		file.seekg (0, ios::beg);
		file.read((char*)&nVersion,sizeof(nVersion));
		file.read((char*)&m_nNumTriangles,sizeof(m_nNumTriangles));
		float *nVertex = new float[m_nNumTriangles*9];
		float *nTexture = new float[m_nNumTriangles*6];
		float *nNormal = new float[m_nNumTriangles*9];
		float *nTangentbuf = new float[m_nNumTriangles*9];
		float *nBitangentbuf = new float[m_nNumTriangles*9];
		file.read((char*)nVertex,sizeof(float)*9*m_nNumTriangles);
		file.read((char*)nTexture,sizeof(float)*6*m_nNumTriangles);
		file.read((char*)nNormal,sizeof(float)*9*m_nNumTriangles);
		file.read((char*)nTangentbuf,sizeof(float)*9*m_nNumTriangles);
		file.read((char*)nBitangentbuf,sizeof(float)*9*m_nNumTriangles);
		file.read((char*)&m_nRadius,sizeof(m_nRadius));
		file.read((char*)&m_nCenter,sizeof(m_nCenter));

		glGenBuffers(1,&m_nBufferID);
		glBindBuffer(GL_ARRAY_BUFFER,m_nBufferID);
		glBufferData(GL_ARRAY_BUFFER,m_nNumTriangles*42*sizeof(GLfloat), NULL,GL_STATIC_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER,0,m_nNumTriangles*9*sizeof(GLfloat),nVertex);
		glBufferSubData(GL_ARRAY_BUFFER,m_nNumTriangles*9*sizeof(GLfloat),m_nNumTriangles*6*sizeof(GLfloat),nTexture);
		glBufferSubData(GL_ARRAY_BUFFER,m_nNumTriangles*15*sizeof(GLfloat),m_nNumTriangles*9*sizeof(GLfloat),nNormal);
		glBufferSubData(GL_ARRAY_BUFFER,m_nNumTriangles*24*sizeof(GLfloat),m_nNumTriangles*9*sizeof(GLfloat),nTangentbuf);
		glBufferSubData(GL_ARRAY_BUFFER,m_nNumTriangles*33*sizeof(GLfloat),m_nNumTriangles*9*sizeof(GLfloat),nBitangentbuf);

		delete[] nVertex;
		delete[] nTexture;
		delete[] nNormal;
		delete[] nTangentbuf;
		delete[] nBitangentbuf;

		return true;
	}
	return false;
}