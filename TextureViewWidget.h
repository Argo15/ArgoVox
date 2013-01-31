#ifndef TEXTUREVIEWWIDGET_H
#define TEXTUREVIEWWIDGET_H

#include <QGLWidget>
#include <string>
#include "Matrix.h"
#include "View.h"

class TextureViewWidget : public QGLWidget
{
    Q_OBJECT

private:
	std::string texture;
	View *view;

protected:
    void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();

public:
	TextureViewWidget(QWidget *parent = 0, const QGLWidget * shareWidget = 0);
	void setTexture(std::string name);
};

#endif