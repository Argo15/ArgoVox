#include "FileWidget.h"
#include "SceneManager.h"
#include "SaveChunk.h"

FileWidget* FileWidget::m_pInstance = 0;

FileWidget::FileWidget() 
	: QWidget(0)
{
	newSceneBtn = new QPushButton("New Scene");

	chunkCmb = new QComboBox();
	addChunkToSceneBtn = new QPushButton("Add Chunk");

	tileCmb = new QComboBox();
	addTileToSceneBtn = new QPushButton("Add Tile");

	saveAsChunkBtn = new QPushButton("Save as Chunk");
	saveAsTileBtn = new QPushButton("Save as Tile");

	QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *chunkLayout = new QHBoxLayout;
		QHBoxLayout *tileLayout = new QHBoxLayout;
		QHBoxLayout *saveLayout = new QHBoxLayout;

	chunkLayout->addWidget(chunkCmb);
	chunkLayout->addWidget(addChunkToSceneBtn);

	tileLayout->addWidget(tileCmb);
	tileLayout->addWidget(addTileToSceneBtn);

	saveLayout->addWidget(saveAsChunkBtn);
	saveLayout->addWidget(saveAsTileBtn);

	mainLayout->addWidget(newSceneBtn);
	mainLayout->addLayout(chunkLayout);
	mainLayout->addLayout(tileLayout);
	mainLayout->addLayout(saveLayout);
	mainLayout->setContentsMargins(0,0,0,0);

	setLayout(mainLayout);

	connect(newSceneBtn, SIGNAL(clicked()),this, SLOT(newScene()));
	connect(addChunkToSceneBtn, SIGNAL(clicked()),this, SLOT(addChunk()));
	connect(addTileToSceneBtn, SIGNAL(clicked()),this, SLOT(addTile()));
	connect(saveAsChunkBtn, SIGNAL(clicked()),this, SLOT(saveChunk()));
	connect(saveAsTileBtn, SIGNAL(clicked()),this, SLOT(saveTile()));

	setFixedHeight(sizeHint().height());
	setFixedWidth(250);
}

FileWidget *FileWidget::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new FileWidget();
	}
	return m_pInstance;
}


void FileWidget::refresh() {
	tileCmb->clear();
	std::map<std::string, Tile*> *tiles = SceneManager::getInstance()->getTileManager()->getTiles();
	for (map<std::string, Tile*>::iterator it = tiles->begin(); it != tiles->end(); it++) {
		tileCmb->addItem((*it).first.c_str());
	}

	chunkCmb->clear();
	std::map<std::string, Chunk*> *chunks = SceneManager::getInstance()->getChunkManager()->getChunks();
	for (map<std::string, Chunk*>::iterator it = chunks->begin(); it != chunks->end(); it++) {
		chunkCmb->addItem((*it).first.c_str());
	}
}

void FileWidget::newScene() {
	SceneManager::getInstance()->clear();
}

void FileWidget::addChunk() {
	QString value = chunkCmb->currentText();
	SceneManager::getInstance()->addChunk(value.toStdString());
}

void FileWidget::addTile() {
	QString value = tileCmb->currentText();
	SceneManager::getInstance()->addTile(value.toStdString());
}

void FileWidget::saveChunk() {
	QString filename =  QFileDialog::getSaveFileName(this, "Save Tile","Data/Chunks",tr("*.achk"));
	if (!filename.isEmpty()){
		SaveChunk save;
		save.save(qPrintable(filename));
	}
}

void FileWidget::saveTile() {
	QString filename =  QFileDialog::getSaveFileName(this, "Save Tile","Data/Tiles",tr("*.atl"));
	if (!filename.isEmpty()){
		SaveChunk save;
		save.save(qPrintable(filename));
	}
}