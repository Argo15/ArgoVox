#include "VoxelWidget.h"
#include "VoxelGrid.h"
#include "Profiler.h"

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

	profileTbl = new QTableWidget(3, 3, this);
	profileTbl->setVerticalHeaderItem(0,new QTableWidgetItem("Build Voxels"));
	profileTbl->setVerticalHeaderItem(1,new QTableWidgetItem("Reflection"));
	profileTbl->setVerticalHeaderItem(2,new QTableWidgetItem("Indirect Lighting"));
	profileTbl->setHorizontalHeaderItem(0,new QTableWidgetItem("Avg"));
	profileTbl->setHorizontalHeaderItem(1,new QTableWidgetItem("Min"));
	profileTbl->setHorizontalHeaderItem(2,new QTableWidgetItem("Max"));
	profileTbl->setColumnWidth(0,83);
	profileTbl->setColumnWidth(1,83);
	profileTbl->setColumnWidth(2,83);
	profileTbl->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	profileTbl->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	profileTbl->setFixedHeight(profileTbl->verticalHeader()->sizeHint().height()*4);

	QVBoxLayout *mainLayout = new QVBoxLayout;
		QHBoxLayout *mipLayout = new QHBoxLayout;

	mipLayout->addWidget(mipLbl);
	mipLayout->addWidget(mipSld);
	mipLayout->setContentsMargins(0,0,0,0);

	mainLayout->addLayout(mipLayout);
	mainLayout->addWidget(profileTbl);

	setLayout(mainLayout);

	connect(mipSld, SIGNAL(valueChanged(int)),this, SLOT(mipChanged(int)));

	setFixedHeight(mainLayout->sizeHint().height());
	setFixedWidth(400);
	refresh();
}

void VoxelWidget::refresh()
{
	profileTbl->setItem(0, 0, new QTableWidgetItem(QString::number(Profiler::getInstance()->getAverage("Build Voxels") / 1000.0f)));
	profileTbl->setItem(0, 1, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMinimum("Build Voxels") / 1000.0f)));
	profileTbl->setItem(0, 2, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMaximum("Build Voxels") / 1000.0f)));
	profileTbl->setItem(1, 0, new QTableWidgetItem(QString::number(Profiler::getInstance()->getAverage("Reflection") / 1000.0f)));
	profileTbl->setItem(1, 1, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMinimum("Reflection") / 1000.0f)));
	profileTbl->setItem(1, 2, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMaximum("Reflection") / 1000.0f)));
	profileTbl->setItem(2, 0, new QTableWidgetItem(QString::number(Profiler::getInstance()->getAverage("Indirect Lighting") / 1000.0f)));
	profileTbl->setItem(2, 1, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMinimum("Indirect Lighting") / 1000.0f)));
	profileTbl->setItem(2, 2, new QTableWidgetItem(QString::number(Profiler::getInstance()->getMaximum("Indirect Lighting") / 1000.0f)));
}

void VoxelWidget::mipChanged(int value)
{
	VoxelGrid::getInstance()->setMipLevel(value);
}