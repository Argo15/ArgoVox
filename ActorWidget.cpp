#include "ActorWidget.h"
#include "SceneManager.h"
#include "MaterialManager.h"
#include "ModelManager.h"

ActorWidget* ActorWidget::m_pInstance = 0;

ActorWidget::ActorWidget() 
	: QWidget(0)
{
	matCmb = new QComboBox;
	modelCmb = new QComboBox;

	posXEdt = new QLineEdit;
	posYEdt = new QLineEdit;
	posZEdt = new QLineEdit;
	rotXEdt = new QLineEdit;
	rotYEdt = new QLineEdit;
	rotZEdt = new QLineEdit;
	scaleXEdt = new QLineEdit;
	scaleYEdt = new QLineEdit;
	scaleZEdt = new QLineEdit;

	tableWidget = new QTableWidget(4, 3, this);
	tableWidget->horizontalHeader()->hide();
	modelHdr = new QTableWidgetItem("Model");
	materialHdr = new QTableWidgetItem("Material");
	transHdr = new QTableWidgetItem("Position");
	scaleHdr = new QTableWidgetItem("Scale");
	tableWidget->setVerticalHeaderItem(0,modelHdr);
	tableWidget->setVerticalHeaderItem(1,materialHdr);
	tableWidget->setVerticalHeaderItem(2,transHdr);
	tableWidget->setVerticalHeaderItem(3,scaleHdr);
	tableWidget->setSpan(0,0,1,3);
	tableWidget->setSpan(1,0,1,3);
	tableWidget->setColumnWidth(0,60);
	tableWidget->setColumnWidth(1,60);
	tableWidget->setColumnWidth(2,60);
	tableWidget->resizeRowsToContents();
	tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	tableWidget->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Fixed);
	tableWidget->setFixedHeight(tableWidget->verticalHeader()->sizeHint().height()*4);

	tableWidget->setCellWidget(0,0,modelCmb);
	tableWidget->setCellWidget(1,0,matCmb);
	tableWidget->setCellWidget(2,0,posXEdt);
	tableWidget->setCellWidget(2,1,posYEdt);
	tableWidget->setCellWidget(2,2,posZEdt);
	tableWidget->setCellWidget(3,0,scaleXEdt);
	tableWidget->setCellWidget(3,1,scaleYEdt);
	tableWidget->setCellWidget(3,2,scaleZEdt);

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(tableWidget);

	setLayout(mainLayout);

	connect(matCmb, SIGNAL(activated(QString)),this, SLOT(currentMatChanged(QString)));
	connect(modelCmb, SIGNAL(activated(QString)),this, SLOT(currentModelChanged(QString)));
	connect(posXEdt, SIGNAL(textEdited(QString)),this, SLOT(changePosition()));
	connect(posYEdt, SIGNAL(textEdited(QString)),this, SLOT(changePosition()));
	connect(posZEdt, SIGNAL(textEdited(QString)),this, SLOT(changePosition()));
	connect(scaleXEdt, SIGNAL(textEdited(QString)),this, SLOT(changeScale()));
	connect(scaleYEdt, SIGNAL(textEdited(QString)),this, SLOT(changeScale()));
	connect(scaleZEdt, SIGNAL(textEdited(QString)),this, SLOT(changeScale()));

	layout()->setContentsMargins(0,0,0,0);
	setFixedHeight(sizeHint().height());
	setFixedWidth(250);
}

ActorWidget *ActorWidget::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new ActorWidget();
	}
	return m_pInstance;
}

void ActorWidget::refresh()
{
	Actor *currentActor = SceneManager::getInstance()->getSelectedActor();
	if (currentActor == 0) 
	{
		this->setEnabled(false);
		return;
	}

	this->setEnabled(true);
	std::string curMat = *(currentActor->getMaterial());
	std::string curMod = *(currentActor->getModel());

	modelCmb->clear();
	std::map<std::string,Model *> *models = ModelManager::getInstance()->getModels();
	for (std::map<std::string, Model*>::iterator it = models->begin(); it != models->end(); it++) {
		modelCmb->addItem((*it).first.c_str());
	}
	modelCmb->setCurrentIndex(modelCmb->findText(QString(curMod.c_str())));

	matCmb->clear();
	std::map<std::string, Material *> *materials = MaterialManager::getInstance()->getMaterials();
	for (std::map<std::string, Material *>::iterator it = materials->begin(); it != materials->end(); it++) {
		matCmb->addItem((*it).first.c_str());
	}
	matCmb->setCurrentIndex(matCmb->findText(QString(curMat.c_str())));

	float *position = currentActor->getTranslate();
	float *scale = currentActor->getScale();

	posXEdt->setText(QString::number(position[0]));
	posYEdt->setText(QString::number(position[1]));
	posZEdt->setText(QString::number(position[2]));
	scaleXEdt->setText(QString::number(scale[0]));
	scaleYEdt->setText(QString::number(scale[1]));
	scaleZEdt->setText(QString::number(scale[2]));
}

void ActorWidget::currentMatChanged(QString mat) {
	Actor *currentActor = SceneManager::getInstance()->getSelectedActor();
	if (currentActor == 0)
		return;
	currentActor->setMaterial(new string(mat.toStdString()));
}

void ActorWidget::currentModelChanged(QString model) {
	Actor *currentActor = SceneManager::getInstance()->getSelectedActor();
	if (currentActor == 0)
		return;
	currentActor->setModel(new string(model.toStdString()));
}

void ActorWidget::changePosition() {
	Actor *currentActor = SceneManager::getInstance()->getSelectedActor();
	if (currentActor == 0)
		return;
	currentActor->setTranslate(posXEdt->text().toFloat(),
						posYEdt->text().toFloat(),
						posZEdt->text().toFloat());
}

void ActorWidget::changeScale() {
	Actor *currentActor = SceneManager::getInstance()->getSelectedActor();
	if (currentActor == 0)
		return;
	currentActor->setScale(scaleXEdt->text().toFloat(),
					scaleYEdt->text().toFloat(),
					scaleZEdt->text().toFloat());
}