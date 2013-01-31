#ifndef FILEWIDGET_H
#define FILEWIDGET_H

#include <QtGui>

class FileWidget : public QWidget
{
    Q_OBJECT

private:
	static FileWidget *m_pInstance;

	QComboBox *chunkCmb, *tileCmb;
	QPushButton *newSceneBtn, *addChunkToSceneBtn, *addTileToSceneBtn, *saveAsChunkBtn, *saveAsTileBtn;

	FileWidget();

public:
	static FileWidget *getInstance();
	void refresh();

public slots:
	void newScene();
	void addChunk();
	void addTile();
	void saveChunk();
	void saveTile();
};

#endif