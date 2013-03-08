#ifndef VOXELWIDGET_H
#define VOXELWIDGET_H

#include <QtGui>

class VoxelWidget : public QWidget
{
    Q_OBJECT

private:
	static VoxelWidget *m_pInstance;

	QLabel *mipLbl, *giLbl;
	QCheckBox *giChk;
	QSlider *mipSld;

	VoxelWidget();

public:
	static VoxelWidget *getInstance();
	void refresh();

public slots:
	void mipChanged(int value);
};

#endif