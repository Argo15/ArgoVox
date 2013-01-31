#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include <QtGUI>
#include <string>
#include "TextureViewWidget.h"

class TextureWidget : public QWidget
{
	Q_OBJECT

private:
	TextureWidget(QGLWidget *glWidget);
	static TextureWidget* pInstance;

	QComboBox *texCmb;
	TextureViewWidget *textureView;

public:
    static TextureWidget *getInstance(QGLWidget *glWidget = 0);

public slots:
	void refresh();

private slots:
	void currentTexChanged(QString tex);
};

#endif