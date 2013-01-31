#include <QtGui>
#include "chunkeditor2.h"
#include "MainGraphicsWidget.h"
#include "DockedWidget.h"
#include "NoState.h"
#include "SceneManager.h"
#include "ModelWidget.h"
#include "TextureWidget.h"
#include "MaterialWidget.h"

bool postInit = true;

ChunkEditor2::ChunkEditor2(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	QGLFormat fmt;
	fmt.setDirectRendering(true);
	fmt.setDoubleBuffer(true);
	glWidget = new MainGraphicsWidget(fmt,this);
	setCentralWidget(glWidget);
	startTimer(0);

	dockedWidget = new DockedWidget(glWidget);
	
	addDockWidget(Qt::RightDockWidgetArea, dockedWidget);
}

ChunkEditor2::~ChunkEditor2()
{

}

void ChunkEditor2::timerEvent(QTimerEvent *event)
{
	if (postInit) {
		ModelWidget::getInstance()->refresh();
		TextureWidget::getInstance()->refresh();
		MaterialWidget::getInstance()->refresh("Default");
		postInit = false;
	}
	glWidget->repaint();
}

void ChunkEditor2::closeEvent(QCloseEvent *event)
{
	exit(0);
}