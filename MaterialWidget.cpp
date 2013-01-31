#include "MaterialWidget.h"
#include "ModelWidget.h"
#include "MaterialManager.h"

MaterialWidget *MaterialWidget::pInstance = 0;

MaterialWidget *MaterialWidget::getInstance(QGLWidget *glWidget)
{
	if (pInstance == 0) {
		pInstance=new MaterialWidget(glWidget);
	}
	return pInstance;
}

MaterialWidget::MaterialWidget(QGLWidget *glWidget) : QWidget(0)
{
	materialView = new MaterialViewWidget(this, glWidget);
	materialView->setFixedWidth(195);
	materialView->setFixedHeight(195);

	matCmb = new QComboBox();
	textureCmb = new QComboBox();
	normalCmb = new QComboBox();

	newMaterialBtn = new QPushButton("New Material");
	saveBtn = new QPushButton("Save All");
	colorBtn = new QPushButton("...");
	colorBtn->setFixedWidth(50);
	glowBtn = new QPushButton("...");
	glowBtn->setFixedWidth(50);

	nameLbl			= new QLabel("Name:");
	colorLbl		= new QLabel("Color:");
	specLbl			= new QLabel("Specular:");
	shineLbl		= new QLabel("Shine:");
	glowLbl			= new QLabel("Glow: ");
	colorViewLbl	= new QLabel("                  ");
	glowViewLbl		= new QLabel("                  ");
	colorViewLbl->setFixedWidth(100);
	colorViewLbl->setAutoFillBackground(true);
	glowViewLbl->setFixedWidth(100);
	glowViewLbl->setAutoFillBackground(true);
	textureLbl		= new QLabel("Texture:");
	uvOffLbl		= new QLabel("uvOff:   ");
	uvScaleLbl		= new QLabel("uvScale:");
	texRotateLbl	= new QLabel("uvRot:");
	normalLbl		= new QLabel("Normal:");

	normalChk = new QCheckBox("Normal:");

	nameEdt = new QLineEdit("test");
	uTexOffEdt = new QLineEdit("0");
	uTexOffEdt->setFixedWidth(40);
	vTexOffEdt = new QLineEdit("0");
	vTexOffEdt->setFixedWidth(40);
	uTexScaleEdt = new QLineEdit("1");
	uTexScaleEdt->setFixedWidth(40);
	vTexScaleEdt = new QLineEdit("1");
	vTexScaleEdt->setFixedWidth(40);

	specSld = new QSlider(Qt::Horizontal);
	specSld->setRange(0,255);
	shineSld = new QSlider(Qt::Horizontal);
	shineSld->setRange(5,128);
	texRotateSld = new QSlider(Qt::Horizontal);
	texRotateSld->setRange(0,360);

	QHBoxLayout *mainLayout = new QHBoxLayout;
		QVBoxLayout *leftLayout = new QVBoxLayout;
		QVBoxLayout *rightLayout = new QVBoxLayout;
			QHBoxLayout *nameLayout = new QHBoxLayout;
			QHBoxLayout *colorLayout = new QHBoxLayout;
			QHBoxLayout *glowLayout = new QHBoxLayout;
			QHBoxLayout *specLayout = new QHBoxLayout;
			QHBoxLayout *shineLayout = new QHBoxLayout;
			QHBoxLayout *textureLayout = new QHBoxLayout;
			QHBoxLayout *normalLayout = new QHBoxLayout;
			QHBoxLayout *uvOffLayout = new QHBoxLayout;
			QHBoxLayout *uvScaleLayout = new QHBoxLayout;
			QHBoxLayout *uvRotLayout = new QHBoxLayout;

	leftLayout->addWidget(materialView);
	leftLayout->addWidget(matCmb);
	leftLayout->addWidget(newMaterialBtn);
	leftLayout->addWidget(saveBtn);
	leftLayout->setContentsMargins(0,0,0,0);

	nameLayout->addWidget(nameLbl);
	nameLayout->addWidget(nameEdt);

	colorLayout->addWidget(colorLbl);
	colorLayout->addWidget(colorViewLbl);
	colorLayout->addWidget(colorBtn);
	colorLayout->addStretch();

	glowLayout->addWidget(glowLbl);
	glowLayout->addWidget(glowViewLbl);
	glowLayout->addWidget(glowBtn);
	glowLayout->addStretch();
	
	specLayout->addWidget(specLbl);
	specLayout->addWidget(specSld);

	shineLayout->addWidget(shineLbl);
	shineLayout->addWidget(shineSld);

	textureLayout->addWidget(textureLbl);
	textureLayout->addWidget(textureCmb);

	normalLayout->addWidget(normalChk);
	normalLayout->addWidget(normalCmb);

	uvOffLayout->addWidget(uvOffLbl);
	uvOffLayout->addWidget(uTexOffEdt);
	uvOffLayout->addWidget(vTexOffEdt);
	uvOffLayout->addStretch();

	uvScaleLayout->addWidget(uvScaleLbl);
	uvScaleLayout->addWidget(uTexScaleEdt);
	uvScaleLayout->addWidget(vTexScaleEdt);
	uvScaleLayout->addStretch();

	uvRotLayout->addWidget(texRotateLbl);
	uvRotLayout->addWidget(texRotateSld);

	rightLayout->addLayout(nameLayout);
	rightLayout->addLayout(colorLayout);
	rightLayout->addLayout(glowLayout);
	rightLayout->addLayout(specLayout);
	rightLayout->addLayout(shineLayout);
	rightLayout->addLayout(textureLayout);
	rightLayout->addLayout(normalLayout);
	rightLayout->addLayout(uvOffLayout);
	rightLayout->addLayout(uvScaleLayout);
	rightLayout->addLayout(uvRotLayout);
	rightLayout->addStretch();
	rightLayout->setContentsMargins(0,0,0,0);

	mainLayout->addLayout(leftLayout);
	mainLayout->addLayout(rightLayout);
	mainLayout->setContentsMargins(0,0,0,0);

	setLayout(mainLayout);

	connect(matCmb, SIGNAL(activated(QString)),this, SLOT(currentMatChanged(QString)));
	connect(saveBtn, SIGNAL(clicked()),this, SLOT(saveAll()));
	connect(newMaterialBtn, SIGNAL(clicked()),this, SLOT(addNewMaterial()));
	connect(nameEdt, SIGNAL(textChanged(QString)),this,SLOT(nameChanged(QString)));
	connect(specSld, SIGNAL(valueChanged(int)),this, SLOT(specChanged(int)));
	connect(shineSld, SIGNAL(valueChanged(int)),this, SLOT(shineChanged(int)));
	connect(colorBtn, SIGNAL(clicked()),this, SLOT(colorChanged()));
	connect(glowBtn, SIGNAL(clicked()),this, SLOT(glowChanged()));
	connect(textureCmb, SIGNAL(activated(QString)),this, SLOT(currentTexChanged(QString)));
	connect(normalCmb, SIGNAL(activated(QString)),this, SLOT(currentNormChanged(QString)));
	connect(uTexOffEdt, SIGNAL(textChanged(QString)),this,SLOT(uTexOffChanged(QString)));
	connect(vTexOffEdt, SIGNAL(textChanged(QString)),this,SLOT(vTexOffChanged(QString)));
	connect(uTexScaleEdt, SIGNAL(textChanged(QString)),this,SLOT(uTexScaleChanged(QString)));
	connect(vTexScaleEdt, SIGNAL(textChanged(QString)),this,SLOT(vTexScaleChanged(QString)));
	connect(texRotateSld, SIGNAL(valueChanged(int)),this, SLOT(texRotateChanged(int)));
	connect(normalChk, SIGNAL(clicked()),this, SLOT(normalChanged()));

	setFixedHeight(mainLayout->sizeHint().height());
	setFixedWidth(400);
}


void MaterialWidget::refresh(string matName)
{
	Material *curMat = MaterialManager::getInstance()->getMaterial(matName);

	matCmb->clear();
	std::map<std::string, Material *> *materials = MaterialManager::getInstance()->getMaterials();
	int index=0;
	int matIndex=0;
	for (std::map<std::string, Material *>::iterator it = materials->begin(); it != materials->end(); it++) {
		matCmb->addItem((*it).first.c_str());
		if (matName == (*it).first)
			matIndex=index;
		index++;
	}
	matCmb->setCurrentIndex(matIndex);

	string texName = *(curMat->getTexture());
	string normName = *(curMat->getNormal());

	textureCmb->clear();
	normalCmb->clear();
	index=0;
	int texIndex=0;
	int normIndex=0;
	std::map<std::string,Texture *> *textures = TextureManager::getInstance()->getTextures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++) {
		textureCmb->addItem((*it).first.c_str());
		normalCmb->addItem((*it).first.c_str());
		if (texName == (*it).first)
			texIndex=index;
		if (normName == (*it).first)
			normIndex=index;
		index++;
	}
	textureCmb->setCurrentIndex(texIndex);
	normalCmb->setCurrentIndex(normIndex);

	float *color = curMat->getColor();
	float *emission = curMat->getEmission();
	float specular = curMat->getSpecular();
	int shine = curMat->getShine();
	bool normalsEnabled = curMat->normalsEnabled();

	nameEdt->setText(QString(matName.c_str()));

	QPalette palette;
	QBrush brush;

	brush = QBrush(QColor(color[0]*255, color[1]*255, color[2]*255, 255));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
	colorViewLbl->setPalette(palette);

	brush = QBrush(QColor(emission[0]*255, emission[1]*255, emission[2]*255, 255));
	brush.setStyle(Qt::SolidPattern);
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
	glowViewLbl->setPalette(palette);

	specSld->setValue((int)(specular*255));
	shineSld->setValue(shine);

	if (normalsEnabled)
	{
		normalChk->setCheckState(Qt::Checked);
		normalCmb->setEnabled(true);
	}
	else
	{
		normalChk->setCheckState(Qt::Unchecked);
		normalCmb->setEnabled(false);
	}

	float *uvTexOff = curMat->getTexOffset();
	float *uvTexScale = curMat->getTexScale();
	float texRotate = curMat->getTexRotate();
	uTexOffEdt->setText(QString::number(uvTexOff[0]));
	vTexOffEdt->setText(QString::number(uvTexOff[1]));
	uTexScaleEdt->setText(QString::number(uvTexScale[0]));
	vTexScaleEdt->setText(QString::number(uvTexScale[1]));
	texRotateSld->setValue(texRotate);

	materialView->setMaterial(matName);
	materialView->repaint();
}

Material *MaterialWidget::getActiveMaterial()
{
	return MaterialManager::getInstance()->getMaterial(matCmb->currentText().toStdString());
}

void MaterialWidget::currentMatChanged(QString mat) 
{
	refresh(mat.toStdString());
}

void MaterialWidget::saveAll()
{
	MaterialManager::getInstance()->saveAllMaterials();
}

void MaterialWidget::addNewMaterial()
{
	Material *defaultMat = MaterialManager::getInstance()->getMaterial("Default");
	Material *newMat = new Material();
	*newMat = *defaultMat;
	newMat->setName("New Material");
	string name = MaterialManager::getInstance()->addMaterialSafe(newMat);
	refresh(name);
	ModelWidget::getInstance()->refresh();
}


void MaterialWidget::nameChanged(QString name)
{
	string currentName = *(getActiveMaterial()->getName());
	string newName;
	if (name.toStdString() != currentName)
	{
		newName = MaterialManager::getInstance()->renameMaterial(currentName, name.toStdString());
		matCmb->setItemText(matCmb->findText(currentName.c_str()), newName.c_str());
		materialView->setMaterial(newName);
	}
	ModelWidget::getInstance()->refresh();
}

void MaterialWidget::specChanged(int value)
{
	Material *curMat = getActiveMaterial();
	curMat->setSpecular((float)value/255.0);
	materialView->repaint();
}

void MaterialWidget::shineChanged(int value)
{
	Material *curMat = getActiveMaterial();
	curMat->setShine(value);
	materialView->repaint();
}

void MaterialWidget::colorChanged()
{
	Material *curMat = getActiveMaterial();

	int r,g,b,a;
	QColor color = QColorDialog::getColor(Qt::white,this);
	if (color.isValid())
	{
		color.getRgb (&r, &g, &b);
		curMat->setColor((float)r/255.0,(float)g/255.0,(float)b/255.0);
	}
	materialView->repaint();

	QBrush brush(color);
	brush.setStyle(Qt::SolidPattern);
	QPalette palette;
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
	colorViewLbl->setPalette(palette);
}

void MaterialWidget::glowChanged()
{
	Material *curMat = getActiveMaterial();

	int r,g,b,a;
	QColor color = QColorDialog::getColor(Qt::white,this);
	if (color.isValid())
	{
		color.getRgb (&r, &g, &b);
		curMat->setEmission((float)r/255.0,(float)g/255.0,(float)b/255.0);
	}

	QBrush brush(color);
	brush.setStyle(Qt::SolidPattern);
	QPalette palette;
	palette.setBrush(QPalette::Active, QPalette::Window, brush);
	palette.setBrush(QPalette::Inactive, QPalette::Window, brush);
	glowViewLbl->setPalette(palette);

	materialView->repaint();
}

void MaterialWidget::currentTexChanged(QString tex)
{
	Material *curMat = getActiveMaterial();
	curMat->setTexture(new string(tex.toStdString()));
	materialView->repaint();
}

void MaterialWidget::currentNormChanged(QString tex)
{
	Material *curMat = getActiveMaterial();
	curMat->setNormal(new string(tex.toStdString()));
	materialView->repaint();
}


void MaterialWidget::uTexOffChanged(QString value)
{
	Material *curMat = getActiveMaterial();
	curMat->setUTexOffset(value.toFloat());
	materialView->repaint();
}

void MaterialWidget::vTexOffChanged(QString value)
{
	Material *curMat = getActiveMaterial();
	curMat->setVTexOffset(value.toFloat());
	materialView->repaint();
}

void MaterialWidget::uTexScaleChanged(QString value)
{
	Material *curMat = getActiveMaterial();
	curMat->setUTexScale(value.toFloat());
	materialView->repaint();
}

void MaterialWidget::vTexScaleChanged(QString value)
{
	Material *curMat = getActiveMaterial();
	curMat->setVTexScale(value.toFloat());
	materialView->repaint();
}

void MaterialWidget::texRotateChanged(int value)
{
	Material *curMat = getActiveMaterial();
	curMat->setTexRotate(value);
	materialView->repaint();
}

void MaterialWidget::normalChanged()
{
	Material *curMat = getActiveMaterial();
	bool normalsEnabled = !curMat->normalsEnabled();
	curMat->enableNormal(normalsEnabled);
	if (normalsEnabled)
	{
		normalChk->setCheckState(Qt::Checked);
		normalCmb->setEnabled(true);
	}
	else
	{
		normalChk->setCheckState(Qt::Unchecked);
		normalCmb->setEnabled(false);
	}
	materialView->repaint();
}