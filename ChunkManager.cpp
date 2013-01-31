#include "ChunkManager.h"
#include<stdio.h>
#include<cstdlib>
#include<iostream>
#include<string.h>
#include<fstream>
#include "dirent.h"
#include "Logger.h"

ChunkManager::ChunkManager()
{
	
}

void ChunkManager::initialize()
{
	Logging::GRAPHICS->info("Loading Chunks");
	DIR *pDIR;
	struct dirent *entry;
	if( pDIR=opendir("./Data/Chunks") )
	{
		while(entry = readdir(pDIR))
		{
			if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
			{
				loadChunk(string("./Data/Chunks/") + entry->d_name);
			}
		}
		closedir(pDIR);
	}
	Logging::GRAPHICS->info("Done Loading Chunks");
}

void ChunkManager::loadChunk(string sFileName)
{
	Chunk *chunk = new Chunk();
	if (chunk->loadChunk(sFileName)) 
	{
		int start_pos = sFileName.rfind("/")+1;
		int end_pos = sFileName.rfind(".");
		string sName = sFileName.substr(start_pos,end_pos-start_pos);
		chunk->setName(sName);
		m_chunks[sName]=chunk;
		return;
	} 
	else 
	{
		Logging::GRAPHICS->error("Chunk failed to load: " + sFileName);
	}
}

void ChunkManager::drawChunk(string sName, string sShader)
{
	m_chunks[sName]->drawChunk(sShader);
}

void ChunkManager::deleteChunk(string sName)
{
	if (m_chunks.find(sName) == m_chunks.end())
	{
		delete m_chunks[sName];
		m_chunks.erase(sName);
	}
}

void ChunkManager::deleteAllChunks()
{
	m_chunks.clear();
}

Chunk *ChunkManager::getChunk(string sName) 
{ 
	return m_chunks[sName]; 
}

map<string,Chunk *> *ChunkManager::getChunks() 
{
	return &m_chunks;
}