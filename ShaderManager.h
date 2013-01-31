#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <string>
#include <map>
#include "GLSLProgram.h"
#include "GLSLShader.h"
using namespace std;

class ShaderManager 
{
private:
	static ShaderManager *m_pInstance;
	map<string,GLSLProgram *> m_shaders;
	void compileProgram(GLSLProgram *program, string sShaderName);

	ShaderManager();
public:
	static ShaderManager *getInstance();

    void initialize();
	void loadShader(string sName, string sVertexShader, string sFragmentShader);
	void useShader(string sName);
	void disableShader(string sName);
	void deleteShader(string sName);
	void deleteAllShaders();

	GLSLProgram *getShader(string sName);
	map<string,GLSLProgram *> *getShaders();
};

#endif