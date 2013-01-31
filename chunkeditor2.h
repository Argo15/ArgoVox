#ifndef CHUNKEDITOR2_H
#define CHUNKEDITOR2_H

#include <QtGui/QMainWindow>

class ChunkEditor2 : public QMainWindow
{
	Q_OBJECT

private:
	QGLWidget *glWidget;
	QDockWidget *dockedWidget;

public:
	ChunkEditor2(QWidget *parent = 0, Qt::WFlags flags = 0);
	~ChunkEditor2();

protected:
	void timerEvent(QTimerEvent *event);
	void closeEvent(QCloseEvent *event);
};

#endif 
