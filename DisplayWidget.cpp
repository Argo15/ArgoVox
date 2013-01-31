#include "DisplayWidget.h"
#include "SceneManager.h"

DisplayWidget* DisplayWidget::m_pInstance = 0;

DisplayWidget::DisplayWidget() 
	: QWidget(0)
{
	displayLbl = new QLabel("Display:");
	sceneChk = new QCheckBox("Scene");
	sceneChk->setCheckState(Qt::Checked);
	physicsChk = new QCheckBox("Physics");
	tilesChk = new QCheckBox("Tiles");

	editModeLbl = new QLabel("Edit Mode:");
	editModeCmb = new QComboBox();
	editModeCmb->addItem("Scene");
	editModeCmb->addItem("Physics");
	editModeCmb->addItem("Tiles");

	QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *displayLayout1 = new QHBoxLayout;
		QHBoxLayout *displayLayout2 = new QHBoxLayout;
		QHBoxLayout *editLayout = new QHBoxLayout;

	displayLayout1->addWidget(displayLbl);
	displayLayout1->addWidget(sceneChk);

	displayLayout2->addWidget(physicsChk);
	displayLayout2->addWidget(tilesChk);

	editLayout->addWidget(editModeLbl);
	editLayout->addWidget(editModeCmb);

	mainLayout->addLayout(displayLayout1);
	mainLayout->addLayout(displayLayout2);
	mainLayout->addLayout(editLayout);
	mainLayout->addStretch();

	setLayout(mainLayout);
	layout()->setContentsMargins(5,0,0,0);

	connect(sceneChk, SIGNAL(clicked()),this, SLOT(sceneChecked()));
	connect(physicsChk, SIGNAL(clicked()),this, SLOT(physicsChecked()));
	connect(tilesChk, SIGNAL(clicked()),this, SLOT(tilesChecked()));
	connect(editModeCmb, SIGNAL(activated(QString)),this, SLOT(currentModeChanged(QString)));

	setFixedHeight(sizeHint().height());
	setFixedWidth(150);
}

DisplayWidget *DisplayWidget::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new DisplayWidget();
	}
	return m_pInstance;
}

void DisplayWidget::sceneChecked() {
	SceneManager::getInstance()->setDisplayScene(!SceneManager::getInstance()->getDisplayScene());
}

void DisplayWidget::physicsChecked() {
	SceneManager::getInstance()->setDisplayPhysics(!SceneManager::getInstance()->getDisplayPhysics());
}

void DisplayWidget::tilesChecked() {
	SceneManager::getInstance()->setDisplayTiles(!SceneManager::getInstance()->getDisplayTiles());
}


void DisplayWidget::currentModeChanged(QString mode) {
	if (mode == "Scene") {
		SceneManager::getInstance()->setEditMode(0);
	} else if (mode == "Physics") {
		SceneManager::getInstance()->setEditMode(1);
	} else if (mode == "Tiles") {
		SceneManager::getInstance()->setEditMode(2);
	}

}