#include "CascadedShadowMap.h"
#include "Profiler.h"
#include "GameState.h"
#include "MatrixManager.h"
#include "ShaderManager.h"
#include "SceneManager.h"

#define ANG2RAD 3.14159265358979323846/180.0

CascadedShadowMap::CascadedShadowMap()
{
	CascadedShadowMap(1024);
}

CascadedShadowMap::CascadedShadowMap(int nSize, float nSlice1, float nSlice2, float nSlice3)
{
	for (int i=0;i<4;i++)
	{
		m_shadowMaps[i] = new DepthBuffer(nSize,nSize);
	}
	m_nSlices[0] = nSlice1;
	m_nSlices[1] = nSlice2;
	m_nSlices[2] = nSlice3;

	this->m_nSize = nSize;
}

void CascadedShadowMap::buildShadowMaps(Camera *camera, View *view, DirectLight *light)
{
	//Profiler::getInstance()->startProfile("Build Shadow Maps");
	float nSlice[] = {0.0, m_nSlices[0], m_nSlices[1], m_nSlices[2], 1.0};
	Frustum *frustum = new Frustum();
	frustum->getFrustum(camera, view);

	m_shadowMaps[3]->bind();
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glDisable(GL_CULL_FACE);
	for (int i=0; i<4; i++)
	{
		Camera *lightCamera = createLightCamera(nSlice[i],nSlice[i+1],camera,view,light);
		View *lightView = createLightView(nSlice[i],nSlice[i+1],camera,lightCamera,view,frustum);
		Frustum *lightFrustum = new Frustum();
		lightFrustum->getOrthoFrustum(lightCamera,lightView);

		MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
		MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
		
		m_shadowMaps[i]->bind();
		glClearDepth(1.0);
		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		glPushAttrib( GL_VIEWPORT_BIT );
		glViewport( 0, 0, m_nSize, m_nSize);
		lightView->use3D(false);

		GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("DirectShadow");
		glslProgram->use();

		
		glm::mat4 cameraMat = glm::mat4(1.0f);
		cameraMat = lightCamera->transformToMatrix(cameraMat);

		m_m4LightMatrix[i] = MatrixManager::getInstance()->getMatrix4(PROJECTION) * cameraMat;
		MatrixManager::getInstance()->putMatrix4(MODELVIEW, cameraMat);

		glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
		glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");

		SceneManager::getInstance()->draw("DirectShadow");
		glslProgram->disable();

		glPopAttrib();
		m_shadowMaps[i]->unbind();

		delete lightCamera;
		delete lightView;
		delete lightFrustum;
	}
	glEnable(GL_CULL_FACE);
	//Profiler::getInstance()->endProfile();
}

Camera *CascadedShadowMap::createLightCamera(float nSlice1, float nSlice2, Camera *camera, View *view, DirectLight *dLight)
{
	Vector3 lightLookAt = dLight->getDirection();
	Vector3 lightRight = dLight->getRight();
	Vector3 lightUp = dLight->getUp();
	float viewDepth = (float)(view->getNear()+(view->getFar()-view->getNear())*(nSlice2+nSlice1)/2.0);
	Vector3 cameraDir = camera->getLookAt()-camera->getEyeV();
	cameraDir.normalize();
	Vector3 lightLookPoint = camera->getEyeV()+cameraDir*viewDepth;
	Vector3 lightPos = lightLookPoint-lightLookAt*25.0;
	Camera *lightCamera = new Camera();
	lightCamera->setPosition(lightPos[0],lightPos[1],lightPos[2]);
	lightCamera->setLookAt(lightLookPoint[0],lightLookPoint[1],lightLookPoint[2]);
	lightCamera->setUp(lightUp[0],lightUp[1],lightUp[2]);
	lightCamera->setRight(lightRight[0],lightRight[1],lightRight[2]);
	return lightCamera;
}

View *CascadedShadowMap::createLightView(float nSlice1, float nSlice2, Camera *camera, Camera *lightCamera, View *view, Frustum *frustum)
{
	glm::vec3 ntl,ntr,nbl,nbr,ftl,ftr,fbl,fbr,nc,fc,X,Y,Z;

	float nearDepth = (float)(view->getNear()+(view->getFar()-view->getNear())*nSlice1);
	float farDepth = (float)(view->getNear()+(view->getFar()-view->getNear())*nSlice2);

	float nh = (frustum->getHeightNear()+(frustum->getHeightFar()-frustum->getHeightNear())*nSlice1)/2.0f;
	float nw = (frustum->getWidthNear()+(frustum->getWidthFar()-frustum->getWidthNear())*nSlice1)/2.0f; 
	float fh = (frustum->getHeightNear()+(frustum->getHeightFar()-frustum->getHeightNear())*nSlice2)/2.0f;
	float fw = (frustum->getWidthNear()+(frustum->getWidthFar()-frustum->getWidthNear())*nSlice2)/2.0f;

	glm::vec3 cameraRight(camera->getRight()[0], camera->getRight()[1], camera->getRight()[2]);
	glm::vec3 cameraUp(camera->getUp()[0], camera->getUp()[1], camera->getUp()[2]);
	glm::vec3 cameraLookAt(camera->getLookAt()[0],camera->getLookAt()[1],camera->getLookAt()[2]);
	glm::vec3 cameraEye(camera->getEyeV()[0],camera->getEyeV()[1],camera->getEyeV()[2]);

	X = cameraRight;
	X = glm::normalize(X);
	Y = cameraUp;
	Y = glm::normalize(Y);
	Z = cameraLookAt - cameraEye;
	Z = glm::normalize(Z);

	nc = cameraEye+Z*nearDepth;
	fc = cameraEye+Z*farDepth;

	ntl = nc + Y * nh - X * nw;
	ntr = nc + Y * nh + X * nw;
	nbl = nc - Y * nh - X * nw;
	nbr = nc - Y * nh + X * nw;
	ftl = fc + Y * fh - X * fw;
	ftr = fc + Y * fh + X * fw;
	fbl = fc - Y * fh - X * fw;
	fbr = fc - Y * fh + X * fw;

	// calculate the actual bounds for the ortho projection
	glm::mat4 lightMVP;
	lightMVP = glm::ortho(-1,1,-1,1,0,1);
	lightMVP = lightCamera->transformToMatrix(lightMVP);

	float left, right, up, down, back;

	glm::vec4 corners[] = {glm::vec4(ntl,1.0f), glm::vec4(ntr,1.0f), glm::vec4(nbl,1.0f), glm::vec4(nbr,1.0f),
							glm::vec4(ftl,1.0f), glm::vec4(ftr,1.0f), glm::vec4(fbl,1.0f), glm::vec4(fbr,1.0f)};

	// get the projected positions of the corners
	for (int i=0; i<8; i++)
	{
		corners[i]=lightMVP*corners[i];
	}


	left  = corners[0][0];
	right = corners[0][0];
	up    = corners[0][1];
	down  = corners[0][1];
	back  = corners[0][2];

	// determine the bounds
	for (int i=1; i<8; i++)
	{
		if (corners[i][0]<left)
		{
			left=corners[i][0];
		}
		if (corners[i][0]>right)
		{
			right=corners[i][0];
		}
		if (corners[i][1]<down)
		{
			down=corners[i][1];
		}
		if (corners[i][1]>up)
		{
			up=corners[i][1];
		}
		if (corners[i][2]>back)
		{
			back=corners[i][2];
		}
	}

	View *lightView = new View();
	lightView->set2D(left,right,down,up,0.1,50);
	return lightView;
}

void CascadedShadowMap::sendToShader(string sShader)
{
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader(sShader);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,m_shadowMaps[0]->getTexture());
	glslProgram->sendUniform("shadowMap[0]",4);
	glActiveTexture(GL_TEXTURE5);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_shadowMaps[1]->getTexture());
	glslProgram->sendUniform("shadowMap[1]",5);
	glActiveTexture(GL_TEXTURE6);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_shadowMaps[2]->getTexture());
	glslProgram->sendUniform("shadowMap[2]",6);
	glActiveTexture(GL_TEXTURE7);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D,m_shadowMaps[3]->getTexture());
	glslProgram->sendUniform("shadowMap[3]",7);

	glslProgram->sendUniform("slices[0]",m_nSlices[0]);
	glslProgram->sendUniform("slices[1]",m_nSlices[1]);
	glslProgram->sendUniform("slices[2]",m_nSlices[2]);

	m_m4LightMatrix[0] = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * m_m4LightMatrix[0];
	m_m4LightMatrix[1] = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * m_m4LightMatrix[1];
	m_m4LightMatrix[2] = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * m_m4LightMatrix[2];
	m_m4LightMatrix[3] = glm::translate(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * glm::scale(glm::mat4(1.0f),glm::vec3(0.5f,0.5f,0.5f)) * m_m4LightMatrix[3];

	glslProgram->sendUniform("lightMatrix[0]", &m_m4LightMatrix[0][0][0]);
	glslProgram->sendUniform("lightMatrix[1]", &m_m4LightMatrix[1][0][0]);
	glslProgram->sendUniform("lightMatrix[2]", &m_m4LightMatrix[2][0][0]);
	glslProgram->sendUniform("lightMatrix[3]", &m_m4LightMatrix[3][0][0]);
}

void CascadedShadowMap::bindShadowMap(int nMap)
{
	glBindTexture(GL_TEXTURE_2D,m_shadowMaps[nMap]->getTexture());
}