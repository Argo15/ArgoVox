#include "ModelViewWidget.h"
#include "ShaderManager.h"
#include "MaterialManager.h"

ModelViewWidget::ModelViewWidget(QWidget *parent, const QGLWidget *shareWidget)
 : QGLWidget(parent,shareWidget)
{
	setFocusPolicy(Qt::StrongFocus);
}

void ModelViewWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);				// GL_SMOOTH or GL_FLAT
	glClearDepth(1.0f);						// depth cleared to
	glEnable(GL_DEPTH_TEST);				// enable depth testing
	glEnable(GL_TEXTURE_2D);				// enable texturing
	glDepthFunc(GL_LEQUAL);					// lesser than or equal to depth
	
	view = new View();
	actor = new Actor(new std::string("Cube"), new std::string("Default"));

	camera = new QtCamera();
	camera->setLookAt(0,0,0);
	camera->setPosition(3,3,3);
	camera->setUp(0,1,0);
}

void ModelViewWidget::resizeGL(int width, int height)
{
	view->viewport(0,0,width,height);
	GLfloat aspect = GLfloat(width) / height;
	view->set3D(45.0f,aspect,0.1,100);
}

void ModelViewWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	MatrixManager::getInstance()->putMatrix4(MODELVIEW, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix4(PROJECTION, glm::mat4(1.0f));
	MatrixManager::getInstance()->putMatrix3(NORMAL, glm::mat3(1.0f));
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

	glslProgram->sendUniform("light.direction", -5.0f,-5.0f,-5.0f);
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

	if (MaterialManager::getInstance()->hasMaterial(*(actor->getMaterial())))
	{
		MatrixManager::getInstance()->pushMatrix4(MODELVIEW);
		MatrixManager::getInstance()->pushMatrix3(NORMAL);
			MatrixManager::getInstance()->putMatrix4(MODELVIEW, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix4(MODELVIEW)));
			MatrixManager::getInstance()->putMatrix3(NORMAL, actor->transformToMatrix(MatrixManager::getInstance()->getMatrix3(NORMAL)));
			actor->drawActor("Basic");
		MatrixManager::getInstance()->popMatrix4(MODELVIEW);
		MatrixManager::getInstance()->popMatrix3(NORMAL);
	}

	glslProgram->disable();
}

void ModelViewWidget::setMaterial(std::string name)
{
	std::string *material = new string(name);
	actor->setMaterial(material);
	repaint();
}

void ModelViewWidget::setModel(std::string name)
{
	std::string *model = new std::string(name);
	actor->setModel(model);
	cameraDist=2.0 * actor->getScaledRadius();
	camera->setPosition(cameraDist,cameraDist,cameraDist);
	repaint();
}