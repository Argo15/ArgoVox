#include "MainGraphicsWidget.h"
#include "Matrix.h"
#include "NoState.h"
#include "QtCamera.h"
#include "SceneManager.h"
#include "FileWidget.h"
#include "ModelWidget.h"
#include "DrawFunc.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "MaterialManager.h"
#include "ShaderManager.h"
#include "InputManager.h"
#include "RenderState.h"
#include "NoState.h"

MainGraphicsWidget::MainGraphicsWidget(QGLFormat fmt, QWidget *parent)
    : QGLWidget(fmt,parent) 
{
	fps = 60; // temp, unless it causes no problems
	setFormat(fmt);
	this->setMouseTracking(true);
	setFocusPolicy(Qt::StrongFocus);
	this->setAutoBufferSwap(true);
}

void MainGraphicsWidget::initializeGL() {
	GameState::GAMESTATE = new NoState();
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	TextureManager::getInstance()->initialize();
	ModelManager::getInstance()->initialize();
	MaterialManager::getInstance()->initialize();
	ShaderManager::getInstance()->initialize();

	glShadeModel(GL_SMOOTH);
	glClearDepth(1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
	glEnable(GL_NORMALIZE);
	glDisable(GL_CULL_FACE);
	glEnable(GL_LIGHTING);

	view = new View();
	camera = new QtCamera();
	camera->setPosition(5,5,5);
	camera->setLookAt(0,0,0);
	camera->setUp(0,1,0);

	FileWidget::getInstance()->refresh();

	myGrid = new Grid(16, 16);
	myGrid->setColor(1.0, 1.0 ,1.0);

	m_gBuffer = new GBuffer(1280,720);
	m_lightBuffer = new LightBuffer(1280,720);
	m_finalBuffer = new FinalBuffer(1280,720);
}

void MainGraphicsWidget::resizeGL(int width, int height) {
	if(height == 0)
		height = 1;
	GLfloat aspect = GLfloat(width) / height;
	view->viewport(0, 0, width, height);
	view->set3D(45.0f,aspect,0.01,50);
	view->set2D(0,1,0,1,0,1);
}

void MainGraphicsWidget::update(float fps) {
	camera->move(60/fps);
}

void MainGraphicsWidget::forwardRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	view->viewport();
	view->use3D(true);

	camera->transform();
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("Basic");
	glslProgram->use();

	glBindFragDataLocation(glslProgram->getHandle(), 0, "fragColor");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");
	glBindAttribLocation(glslProgram->getHandle(), 1, "v_texture");
	glBindAttribLocation(glslProgram->getHandle(), 2, "v_normal");
	glBindAttribLocation(glslProgram->getHandle(), 3, "v_tangent");
	glBindAttribLocation(glslProgram->getHandle(), 4, "v_bitangent");

	glslProgram->sendUniform("light.direction", 1.0f,-5.0f,2.0f);
	glslProgram->sendUniform("light.color", 1.0f,1.0f,1.0f);
	glslProgram->sendUniform("light.ambient", 0.7f);
	glslProgram->sendUniform("light.diffuse", 0.6f);
	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	glslProgram->sendUniform("curveGeometry", false);

	glm::mat4 cameraInverse = glm::mat4(1.0);
	cameraInverse = camera->transformToMatrix(cameraInverse);
	cameraInverse = glm::inverse(cameraInverse);
	glslProgram->sendUniform("inverseCameraMatrix", &cameraInverse[0][0]);
	glslProgram->sendUniform("cameraPos", camera->getEyeX(), camera->getEyeY(), camera->getEyeZ());

	MaterialManager::getInstance()->getMaterial("Default")->sendToShader("Basic");
	myGrid->draw();

	SceneManager::getInstance()->draw("Basic");

	glslProgram->disable();

	SceneManager::getInstance()->drawTransformers();
}

void MainGraphicsWidget::voxelRender()
{
	VoxelGrid::getInstance()->clear();
	VoxelGrid::getInstance()->buildVoxels(view, camera);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
	view->use3D(true);

	camera->transform();
	GLSLProgram *glslProgram = ShaderManager::getInstance()->getShader("Voxel");
	glslProgram->use();

	glBindFragDataLocation(glslProgram->getHandle(), 0, "fragColor");
	glBindAttribLocation(glslProgram->getHandle(), 0, "v_vertex");

	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);

	glm::mat4 cameraInverse = glm::mat4(1.0);
	cameraInverse = camera->transformToMatrix(cameraInverse);
	cameraInverse = glm::inverse(cameraInverse);
	glslProgram->sendUniform("invCameraMatrix", &cameraInverse[0][0]);

	VoxelGrid::getInstance()->bind(3);
	glPointSize(10.0f);
	glEnable(GL_POINT_SMOOTH);
	glBegin(GL_POINTS);
	int numVoxels = VOXEL_SIZE;
	float increase = 32.0f / (float)numVoxels;
	for (float x=-7.1; x<8; x+=increase)
	{
		for (float y=-7.1; y<8; y+=increase)
		{
			for (float z=-7.1; z<8; z+=increase)
			{
				glVertex3f(x,y,z);
			}
		}
	}
	glEnd();

	glslProgram->disable();
}
	
void MainGraphicsWidget::deferredRender()
{
	VoxelGrid::getInstance()->clear();
	VoxelGrid::getInstance()->buildVoxels(view, camera);

	m_gBuffer->drawToBuffer(view, camera, myGrid);
	m_lightBuffer->drawToBuffer(m_gBuffer->getNormalTex(), m_gBuffer->getDepthTex(), m_gBuffer->getGlowTex(), view, camera);
	m_finalBuffer->drawToBuffer(m_gBuffer->getColorTex(), m_lightBuffer->getLightTex(), m_lightBuffer->getGlowTex(), view);

	glDisable(GL_LIGHTING);
	glActiveTextureARB(GL_TEXTURE2);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE1);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0);
	glEnable(GL_TEXTURE_2D);

	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, 1.0, 0, 1.0);
	view->viewport();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glBindTexture(GL_TEXTURE_2D, 0);
	if (RenderStateManager::RENDERSTATE == FINAL)
	{
		m_finalBuffer->bindFinalTex();
	}
	if (RenderStateManager::RENDERSTATE == POSITION)
	{
		m_gBuffer->bindDepthTex();
	}
	if (RenderStateManager::RENDERSTATE == NORMALMAP)	
	{
		m_gBuffer->bindNormalTex();
	}
	if (RenderStateManager::RENDERSTATE == COLOR)	
	{
		m_gBuffer->bindColorTex();
	}
	if (RenderStateManager::RENDERSTATE == LIGHTING)
	{
		m_lightBuffer->bindLightTex();
	}
	if (RenderStateManager::RENDERSTATE == SPECULAR)
	{
		m_lightBuffer->bindGlowTex();
	}
	glColor3f(1.0f,1.0f,1.0f);
	drawScreen(0.0,0.0,1.0,1.0);
}

void MainGraphicsWidget::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (RenderStateManager::RENDERSTATE == FORWARD)
	{
		forwardRender();
	}
	else if (RenderStateManager::RENDERSTATE == VOXEL)
	{
		voxelRender();
	}
	else
	{
		deferredRender();
	}
}


void MainGraphicsWidget::keyPressEvent (QKeyEvent *event) {
	InputManager::getInstance()->registerKeyDown(event->key());
	if (event->key() == Qt::Key_Delete) {
		SceneManager::getInstance()->removeSelected();
	}
	if (event->key() >= Qt::Key_0 && event->key() <= Qt::Key_9)
	{
		RenderStateManager::RENDERSTATE = (RenderState)(event->key() - Qt::Key_0);
		repaint();

	}
}

void MainGraphicsWidget::keyReleaseEvent (QKeyEvent *event) {
	InputManager::getInstance()->registerKeyUp(event->key());
}

void MainGraphicsWidget::mousePressEvent(QMouseEvent *event) {
	int x = event->x();
	int y = event->y();
	if (event->buttons() & Qt::LeftButton) {
		if (Transformer::selected == -1)
			SceneManager::getInstance()->setSelectedActor(-1);
		MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
		MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
		view->use3D(true);
		camera->transform();
		if (SceneManager::getInstance()->getEditMode() != 2) {
			Selection::calculateSelection(x,y);
		}
	}
	InputManager::getInstance()->registerMouseButtonDown(event->button());
}

void MainGraphicsWidget::mouseReleaseEvent(QMouseEvent *event) {
	InputManager::getInstance()->registerMouseButtonUp(event->button());
}

void MainGraphicsWidget::mouseMoveEvent(QMouseEvent *event) {
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	view->use3D(true);
	camera->transform();
	int x = event->x();
	int y = event->y();
	InputManager::getInstance()->setMousePosition(x, y);
	if (event->buttons() & Qt::LeftButton){
		transformNoShaders();
		if (SceneManager::getInstance()->getEditMode() == 0) {
			Transformer::calculateTransform(x,y,InputManager::getInstance()->isSpecialKeyDown(Qt::Key_Shift),InputManager::getInstance()->isSpecialKeyDown(Qt::Key_Control));
		} else {
			SceneManager::getInstance()->getSceneTiles()->updateTiles();
		}
	} else {
		Selection::calculateSelectedTransformer(x,y);
		Transformer::dragPoint = Vector3(0.0);
		SceneManager::getInstance()->getSceneTiles()->clearUpdatables();
	}
}

void MainGraphicsWidget::wheelEvent(QWheelEvent *event) {

}
	