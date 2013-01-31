#include "MaterialViewWidget.h"
#include "ShaderManager.h"
#include "MaterialManager.h"
#include "ModelManager.h"

MaterialViewWidget::MaterialViewWidget(QWidget *parent, const QGLWidget *shareWidget)
 : QGLWidget(parent,shareWidget)
{
	setFocusPolicy(Qt::StrongFocus);
}

void MaterialViewWidget::initializeGL()
{
	glShadeModel(GL_SMOOTH);				// GL_SMOOTH or GL_FLAT
	glClearDepth(1.0f);						// depth cleared to
	glEnable(GL_DEPTH_TEST);				// enable depth testing
	glEnable(GL_TEXTURE_2D);				// enable texturing
	glDepthFunc(GL_LEQUAL);					// lesser than or equal to depth
	
	view = new View();

	camera = new QtCamera();
	camera->setLookAt(0,0,0);
	camera->setPosition(0,3,0);
	camera->setUp(0,0,-1);

	material = "Cobble Stone";
}

void MaterialViewWidget::resizeGL(int width, int height)
{
	view->viewport(0,0,width,height);
	GLfloat aspect = GLfloat(width) / height;
	view->set3D(45.0f,aspect,0.1,100);
}

void MaterialViewWidget::paintGL()
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

	glslProgram->sendUniform("light.direction", -5.0f,-5.0f,5.0f);
	glslProgram->sendUniform("light.color", 1.0f,1.0f,1.0f);
	glslProgram->sendUniform("light.ambient", 0.7f);
	glslProgram->sendUniform("light.diffuse", 0.6f);
	glslProgram->sendUniform("projectionMatrix", &MatrixManager::getInstance()->getMatrix4(PROJECTION)[0][0]);
	glslProgram->sendUniform("modelviewMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	glslProgram->sendUniform("normalMatrix", &MatrixManager::getInstance()->getMatrix4(MODELVIEW)[0][0]);
	glslProgram->sendUniform("curveGeometry", false);

	glm::mat4 cameraInverse = glm::mat4(1.0);
	cameraInverse = camera->transformToMatrix(cameraInverse);
	cameraInverse = glm::inverse(cameraInverse);
	glslProgram->sendUniform("inverseCameraMatrix", &cameraInverse[0][0]);
	glslProgram->sendUniform("cameraPos", camera->getEyeX(), camera->getEyeY(), camera->getEyeZ());

	MaterialManager::getInstance()->getMaterial(material)->sendToShader("Basic");
	ModelManager::getInstance()->drawModel("Sphere");

	glslProgram->disable();
}

void MaterialViewWidget::setMaterial(std::string name)
{
	material = name;
	repaint();
}