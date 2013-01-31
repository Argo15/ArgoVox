#ifndef CHUNKMANAGER_H
#define CHUNKMANAGER_H

#include <string>
#include <map>
#include "Chunk.h"
using namespace std;

class ChunkManager 
{
private:
	map<string,Chunk *> m_chunks;

public:
	ChunkManager();

    void initialize();
	void loadChunk(string sFileName);
	void drawChunk(string sName, string sShader);
	void deleteChunk(string sName);
	void deleteAllChunks();

	Chunk *getChunk(string sName);
	map<string,Chunk *> *getChunks();
};

#endif