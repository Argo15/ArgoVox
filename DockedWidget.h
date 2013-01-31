#ifndef DOCKEDWIDGET_H
#define DOCKEDWIDGET_H

#include <QGLWidget>
#include <QtGui>

class DockedWidget : public QDockWidget {
public:
	DockedWidget(QGLWidget *glWidget);
	~DockedWidget(){}
};

#endif