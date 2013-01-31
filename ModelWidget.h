#ifndef MODELWIDGET_H
#define MODELWIDGET_H

#include <QtGUI>
#include <string>
#include "ModelViewWidget.h"

class ModelWidget : public QWidget
{
	Q_OBJECT

private:
	ModelWidget(QGLWidget *glWidget);
	static ModelWidget* pInstance;

	QLabel *modelLbl, *matLbl;
	QComboBox *modelCmb, *matCmb;
	QPushButton *addToSceneBtn;

	ModelViewWidget *modelView;

public:
    static ModelWidget *getInstance(QGLWidget *glWidget = 0);

public slots:
	void refresh();

private slots:
	void currentModelChanged(QString model);
	void currentMatChanged(QString mat);
	void addToScene();
};
#endif