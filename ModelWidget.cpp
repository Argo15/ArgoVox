#include "ModelWidget.h"
#include "Model.h"
#include "Material.h"
#include "Actor.h"
#include "SceneManager.h"
#include "MainGraphicsWidget.h"
#include "MaterialManager.h"
#include "ModelManager.h"

ModelWidget *ModelWidget::pInstance = 0;

ModelWidget *ModelWidget::getInstance(QGLWidget *glWidget)
{
	if (pInstance == 0) {
		pInstance=new ModelWidget(glWidget);
	}
	return pInstance;
}

ModelWidget::ModelWidget(QGLWidget *glWidget) : QWidget(0)
{
	modelView = new ModelViewWidget(this, glWidget);
	modelView->setFixedWidth(195);
	modelView->setFixedHeight(195);

	modelLbl = new QLabel("Mdl:");
	modelLbl->setFixedWidth(30);
	matLbl = new QLabel("Mat:");
	matLbl->setFixedWidth(30);

	modelCmb = new QComboBox();
	matCmb = new QComboBox();

	addToSceneBtn = new QPushButton("Add To Scene");

	QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *modelLayout = new QHBoxLayout;
		QHBoxLayout *materialLayout = new QHBoxLayout;

	modelLayout->addWidget(modelLbl);
	modelLayout->addWidget(modelCmb);

	materialLayout->addWidget(matLbl);
	materialLayout->addWidget(matCmb);

	mainLayout->addWidget(modelView);
	mainLayout->addLayout(modelLayout);
	mainLayout->addLayout(materialLayout);
	mainLayout->addWidget(addToSceneBtn);
	mainLayout->setContentsMargins(0,0,0,0);

	setLayout(mainLayout);

	connect(addToSceneBtn, SIGNAL(clicked()),this, SLOT(addToScene()));
	connect(modelCmb, SIGNAL(activated(QString)),this, SLOT(currentModelChanged(QString)));
	connect(matCmb, SIGNAL(activated(QString)),this, SLOT(currentMatChanged(QString)));

	setFixedHeight(sizeHint().height());
	setFixedWidth(195);
	setContentsMargins(0,0,0,0);
}

void ModelWidget::refresh() {
	modelCmb->clear();
	std::map<std::string,Model *> *models = ModelManager::getInstance()->getModels();
	for (std::map<std::string, Model*>::iterator it = models->begin(); it != models->end(); it++) {
		modelCmb->addItem((*it).first.c_str());
	}

	matCmb->clear();
	std::map<std::string, Material *> *materials = MaterialManager::getInstance()->getMaterials();
	for (std::map<std::string, Material *>::iterator it = materials->begin(); it != materials->end(); it++) {
		matCmb->addItem((*it).first.c_str());
	}

	modelView->setModel(modelCmb->currentText().toStdString());
	modelView->setMaterial(matCmb->currentText().toStdString());

	repaint();
}

void ModelWidget::currentModelChanged(QString model) 
{
	modelView->setModel(model.toStdString());
}

void ModelWidget::currentMatChanged(QString mat) 
{
	modelView->setMaterial(mat.toStdString());
}

void ModelWidget::addToScene() {
	std::string *model = new std::string(modelCmb->currentText().toStdString());
	std::string *material = new std::string(matCmb->currentText().toStdString());
	Actor *actor = new Actor(model, material);
	SceneManager::getInstance()->addActor(actor);
}