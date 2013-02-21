#include "VoxelWidget.h"
#include "VoxelGrid.h"

VoxelWidget *VoxelWidget::m_pInstance = 0;

VoxelWidget *VoxelWidget::getInstance()
{
	if (m_pInstance == 0) {
		m_pInstance=new VoxelWidget();
	}
	return m_pInstance;
}

VoxelWidget::VoxelWidget() : QWidget(0)
{
	mipLbl	= new QLabel("Mip Level:");
	mipSld = new QSlider(Qt::Horizontal);
	mipSld->setRange(0,3);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(mipLbl);
	mainLayout->addWidget(mipSld);
	mainLayout->setContentsMargins(0,0,0,0);

	setLayout(mainLayout);

	connect(mipSld, SIGNAL(valueChanged(int)),this, SLOT(mipChanged(int)));

	setFixedHeight(mainLayout->sizeHint().height());
	setFixedWidth(400);
}


void VoxelWidget::refresh()
{

}

void VoxelWidget::mipChanged(int value)
{
	VoxelGrid::getInstance()->setMipLevel(value);
}