#include "TextureWidget.h"
#include "TextureManager.h"

TextureWidget *TextureWidget::pInstance = 0;

TextureWidget *TextureWidget::getInstance(QGLWidget *glWidget)
{
	if (pInstance == 0) {
		pInstance=new TextureWidget(glWidget);
	}
	return pInstance;
}

TextureWidget::TextureWidget(QGLWidget *glWidget) : QWidget(0)
{
	textureView = new TextureViewWidget(this, glWidget);
	textureView->setFixedWidth(195);
	textureView->setFixedHeight(195);

	texCmb = new QComboBox();
	texCmb->addItem("test");

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(textureView);
	mainLayout->addWidget(texCmb);
	mainLayout->addStretch();
	mainLayout->setContentsMargins(0,0,0,0);

	setLayout(mainLayout);

	connect(texCmb, SIGNAL(activated(QString)),this, SLOT(currentTexChanged(QString)));

	setFixedHeight(285);
	setFixedWidth(195);
	setContentsMargins(0,0,0,0);
}


void TextureWidget::refresh()
{
	texCmb->clear();
	std::map<std::string,Texture *> *textures = TextureManager::getInstance()->getTextures();
	for (std::map<std::string, Texture*>::iterator it = textures->begin(); it != textures->end(); it++) {
		texCmb->addItem((*it).first.c_str());
	}

	currentTexChanged(texCmb->currentText());
}

void TextureWidget::currentTexChanged(QString tex) 
{
	textureView->setTexture(tex.toStdString());
}