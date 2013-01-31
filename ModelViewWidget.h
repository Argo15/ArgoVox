#ifndef MODELVIEWWIDGET_H
#define MODELVIEWWIDGET_H

#include <QGLWidget>
#include "View.h"
#include "Actor.h"
#include "QtCamera.h"

class ModelViewWidget : public QGLWidget
{
    Q_OBJECT

private:
	Actor *actor;
	View *view;
	Camera *camera;
	float cameraDist;

	GLSLProgram *glslProgram;

protected:
    void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

public:
	ModelViewWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0);
	void setMaterial(std::string name);
	void setModel(std::string name);
};

#endif