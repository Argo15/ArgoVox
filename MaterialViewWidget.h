#ifndef MATERIALVIEWWIDGET_H
#define MATERIALVIEWWIDGET_H

#include <QGLWidget>
#include "View.h"
#include "Actor.h"
#include "QtCamera.h"

class MaterialViewWidget : public QGLWidget
{
    Q_OBJECT

private:
	std::string material;
	View *view;
	Camera *camera;

	GLSLProgram *glslProgram;

protected:
    void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

public:
	MaterialViewWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0);
	void setMaterial(std::string name);
};

#endif