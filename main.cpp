#include "chunkeditor2.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	ChunkEditor2 mainWindow;
	mainWindow.setGeometry(80, 50, 1600, 900);
	mainWindow.show();
	return a.exec();
}
