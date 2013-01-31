#include "Chunk.h"
#include "MatrixManager.h"
#include "WorldState.h"

Chunk::Chunk() : Transformable()
{
	m_nNumActors = 0;
	m_actors = 0;
	m_nOrientation = 0;
	m_nRadius = 0;
	for (int i=0; i<10; i++) 
	{
		for (int j=0; j<10; j++) 
		{
			m_nTileModes[i][j] = 0;
		}
	}
}

void Chunk::drawChunk(string sShader) 
{
	MatrixManager::getInstance()->pushMatrix4(MODELVIEW, transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
	MatrixManager::getInstance()->pushMatrix3(NORMAL, transformToMatrix(MatrixManager::getInstance()->getMatrix3(NORMAL)));
		glm::mat4 normMat(MatrixManager::getInstance()->getMatrix3(NORMAL));
		normMat[3] = glm::vec4(0,0,0,1.0f);
		MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::rotate(MatrixManager::getInstance()->getMatrix4(MODELVIEW), 90.0f*m_nOrientation, glm::vec3(0,1.0f,0)));
		normMat = glm::rotate(normMat, 90.0f*m_nOrientation, glm::vec3(0,1.0f,0));
		MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(normMat));
		for (int i=0; i<m_nNumActors; i++) 
		{
			MatrixManager::getInstance()->pushMatrix4(MODELVIEW, m_actors[i]->transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
			MatrixManager::getInstance()->pushMatrix3(NORMAL, m_actors[i]->transformToMatrix(MatrixManager::getInstance()->getMatrix3(NORMAL)));
			m_actors[i]->drawActor(sShader);
			MatrixManager::getInstance()->popMatrix4(MODELVIEW);
			MatrixManager::getInstance()->popMatrix3(NORMAL);
		}
		drawExtra(sShader);
	MatrixManager::getInstance()->popMatrix4(MODELVIEW);
	MatrixManager::getInstance()->popMatrix3(NORMAL);
}

void Chunk::drawExtra(string sShader)
{

}

bool Chunk::loadChunk(string sFileName)
{
	ifstream file(sFileName.c_str(), ios::in|ios::binary|ios::ate);
	if (file.is_open())
	{ 
		file.seekg (0, ios::beg);
		file.read((char*)&m_nNumActors,sizeof(m_nNumActors));
		SaveActor *loadActors = new SaveActor[m_nNumActors];
		m_actors = new Actor*[m_nNumActors];
		file.read((char*)loadActors,sizeof(SaveActor)*m_nNumActors);

		for(int i=0; i<m_nNumActors; i++) 
		{
			string *sModel = ModelManager::getInstance()->getModel(loadActors[i].sModel)->getName();
			string *mat = new string(loadActors[i].sMaterial);
			m_actors[i] = new Actor(sModel, mat);
			m_actors[i]->setName(loadActors[i].sName);
			m_actors[i]->setTranslate(loadActors[i].nTranslation[0],loadActors[i].nTranslation[1],loadActors[i].nTranslation[2]);
			m_actors[i]->setRotate(Quaternion(loadActors[i].nRotation[0],loadActors[i].nRotation[1],loadActors[i].nRotation[2],loadActors[i].nRotation[3]));
			m_actors[i]->setScale(loadActors[i].nScale[0],loadActors[i].nScale[1],loadActors[i].nScale[2]);
			float actorRadius = m_actors[i]->getTranslateV().length()+m_actors[i]->getScaledRadius();
			if (actorRadius>m_nRadius)
			{
				m_nRadius=actorRadius;
			}
		}

		file.read((char*)&m_nNumPhysics,sizeof(m_nNumPhysics));
		SavePhysics *loadPhysics = new SavePhysics[m_nNumPhysics];
		m_physics = new PhysicsShape*[m_nNumPhysics];
		file.read((char*)loadPhysics,sizeof(SavePhysics)*m_nNumPhysics);

		for(int i=0; i<m_nNumPhysics; i++) 
		{
			m_physics[i] = new PhysicsShape(loadPhysics[i].m_physicsType);
			m_physics[i]->setTranslate(loadPhysics[i].nTranslation[0],loadPhysics[i].nTranslation[1],loadPhysics[i].nTranslation[2]);
			m_physics[i]->setRotate(Quaternion(loadPhysics[i].nRotation[0],loadPhysics[i].nRotation[1],loadPhysics[i].nRotation[2],loadPhysics[i].nRotation[3]));
			m_physics[i]->setScale(loadPhysics[i].nScale[0],loadPhysics[i].nScale[1],loadPhysics[i].nScale[2]);
		}

		file.read((char*)m_nTileModes,sizeof(m_nTileModes));

		file.close();

		int start_pos = sFileName.rfind("/")+1;
		int end_pos = sFileName.rfind(".");
		string sName = sFileName.substr(start_pos,end_pos-start_pos);
		setName(sName);

		return true;
	}
	return false;
}

void Chunk::setOrientation(int nOrientation) 
{
	m_nOrientation = nOrientation;
}

int Chunk::getOrientation() 
{
	return m_nOrientation;
}

float Chunk::getRadius()
{
	return m_nRadius;
}

void Chunk::addPhysicsToDynamicWorld(PhysicsManager *physicsManager)
{
	BulletManager *bullet = physicsManager->getBulletManager();
	for (int i=0; i<m_nNumPhysics; i++)
	{
		PhysicsShape physicsShape = *m_physics[i];
		physicsShape.matrix = glm::mat4(1.0f);
		physicsShape.matrix = transformToMatrix(physicsShape.matrix);
		physicsShape.matrix = glm::rotate(physicsShape.matrix, 90.0f*m_nOrientation, glm::vec3(0,1.0f,0));
		glm::mat4 mat = glm::mat4(1.0f);
		mat = glm::translate(mat, glm::vec3(physicsShape.getTranslate()[0],physicsShape.getTranslate()[1],physicsShape.getTranslate()[2]));
		glm::quat rot = glm::quat(physicsShape.getRotate()[0],physicsShape.getRotate()[1],physicsShape.getRotate()[2],physicsShape.getRotate()[3]);
		mat = mat * glm::mat4_cast(rot);
		physicsShape.matrix = physicsShape.matrix * mat;
		bullet->addPhysicsShape(&physicsShape);
	}
}

static int N_CHUNK_MAX_XY = 9; 

int Chunk::getTileMode(int nPosX, int nPosY)
{
	int worldX = nPosX;
	int worldY = nPosY;
	if (m_nOrientation == 3)
	{
		worldX = nPosY;
		worldY = N_CHUNK_MAX_XY-nPosX;
	}
	if (m_nOrientation == 2)
	{
		worldX = N_CHUNK_MAX_XY-nPosX;
		worldY = N_CHUNK_MAX_XY-nPosY;
	}
	if (m_nOrientation == 1)
	{
		worldX = N_CHUNK_MAX_XY-nPosY;
		worldY = nPosX;
	}
	return m_nTileModes[worldX][worldY];
}

Actor **Chunk::getActors() 
{
	return m_actors;
}

int Chunk::getNumActors() 
{
	return m_nNumActors;
}

PhysicsShape **Chunk::getPhysics() 
{
	return m_physics;
}

int Chunk::getNumPhysics() 
{
	return m_nNumPhysics;
}