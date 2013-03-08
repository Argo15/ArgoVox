#include "ShaderManager.h"
#include <assert.h>
#include "Logger.h"

ShaderManager* ShaderManager::m_pInstance = 0;

ShaderManager::ShaderManager()
{

}

ShaderManager *ShaderManager::getInstance() 
{
	if (m_pInstance == 0) 
	{
		m_pInstance = new ShaderManager();
	}
	return m_pInstance;
}

void ShaderManager::compileProgram(GLSLProgram *program, string sShaderName)
{
	std::string sLog;
	if (!program->vertex_->isCompiled())
	{
		program->vertex_->getShaderLog(sLog);
		Logging::GRAPHICS->error("Vertex shader failed to compile: " + sShaderName + "\n" + sLog);
	}
	if (!program->fragment_->isCompiled())
	{	
		program->fragment_->getShaderLog(sLog);
		Logging::GRAPHICS->error("Fragment shader failed to compile " + sShaderName + "\n" + sLog);
	}
}

void ShaderManager::initialize()
{
	Logging::GRAPHICS->info("Loading Shaders");
	loadShader("Basic", "Data/Shaders/v_material.glsl","Data/Shaders/f_material.glsl");
	loadShader("GBuffer", "Data/Shaders/v_GBuffer.glsl","Data/Shaders/f_GBuffer.glsl");
	loadShader("DirectLight", "Data/Shaders/v_light.glsl","Data/Shaders/f_dlight.glsl");
	loadShader("Final", "Data/Shaders/v_final.glsl","Data/Shaders/f_final.glsl");
	loadShader("DirectShadow", "Data/Shaders/v_dShadow.glsl","Data/Shaders/f_dShadow.glsl");
	loadShader("Voxel", "Data/Shaders/v_drawVoxels.glsl","Data/Shaders/f_drawVoxels.glsl");
	loadShader("BuildVoxels", "Data/Shaders/v_buildVoxels.glsl","Data/Shaders/f_buildVoxels.glsl");
	loadShader("GlossyReflection", "Data/Shaders/v_glossy.glsl","Data/Shaders/f_glossy.glsl");
	loadShader("Indirect", "Data/Shaders/v_indirect.glsl","Data/Shaders/f_indirect.glsl");
	loadShader("BuildMipmap", "Data/Shaders/v_buildMipmap.glsl","Data/Shaders/f_buildMipmap.glsl");
	/*loadShader("MotionBlur", "Data/Shaders/v_motionBlur.glsl","Data/Shaders/f_motionBlur.glsl");
	loadShader("Atmosphere", "Data/Shaders/v_motionBlur.glsl","Data/Shaders/f_atmosphere.glsl");*/
	Logging::GRAPHICS->info("Done Loading Shaders");
}

void ShaderManager::loadShader(string sName, string sVertexShader, string sFragmentShader)
{
	GLSLProgram *glslProgram = new GLSLProgram(sVertexShader,sFragmentShader);
	compileProgram(glslProgram, sName);
	m_shaders[sName] = glslProgram;
}

void ShaderManager::useShader(string sName)
{
	m_shaders[sName]->use();
}

void ShaderManager::disableShader(string sName)
{
	m_shaders[sName]->disable();
}

void ShaderManager::deleteShader(string sName)
{
	delete m_shaders[sName];
}

void ShaderManager::deleteAllShaders()
{
	m_shaders.clear();
}

GLSLProgram *ShaderManager::getShader(string sName) 
{ 
	return m_shaders[sName]; 
}

map<string,GLSLProgram *> *ShaderManager::getShaders() 
{
	return &m_shaders;
}