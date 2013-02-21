#include "DockedWidget.h"
#include "FileWidget.h"
#include "ModelWidget.h"
#include "TextureWidget.h"
#include "MaterialWidget.h"
#include "ActorWidget.h"
#include "DisplayWidget.h"
#include "PhysicsWidget.h"
#include "VoxelWidget.h"

DockedWidget::DockedWidget(QGLWidget *glWidget) 
	: QDockWidget(0)
{
	QWidget *mainWidget = new QWidget();
	QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *filePhysicsLayout = new QHBoxLayout;
		QHBoxLayout *modelTextureLayout = new QHBoxLayout;
		QHBoxLayout *actorDisplayLayout = new QHBoxLayout;

	filePhysicsLayout->addWidget(FileWidget::getInstance());

	modelTextureLayout->addWidget(ModelWidget::getInstance(glWidget));
	modelTextureLayout->addWidget(TextureWidget::getInstance(glWidget));

	actorDisplayLayout->addWidget(ActorWidget::getInstance());

	mainLayout->addLayout(filePhysicsLayout);
	mainLayout->addLayout(modelTextureLayout);
	mainLayout->addWidget(MaterialWidget::getInstance(glWidget));
	mainLayout->addLayout(actorDisplayLayout);
	mainLayout->addWidget(VoxelWidget::getInstance());
	mainLayout->setContentsMargins(0,0,0,0);

	mainWidget->setLayout(mainLayout);
	mainWidget->setFixedHeight(mainWidget->sizeHint().height());
	mainWidget->setFixedWidth(mainWidget->sizeHint().width());

	QScrollArea *scrollArea = new QScrollArea;
	scrollArea->setWidget(mainWidget);

	setWidget(scrollArea);
}
