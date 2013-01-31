#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QtGui>

class DisplayWidget : public QWidget
{
	Q_OBJECT

private:
	DisplayWidget();
	static DisplayWidget* m_pInstance;

	QLabel *displayLbl, *editModeLbl;
	QCheckBox *sceneChk, *physicsChk, *tilesChk;
	QComboBox *editModeCmb;


public:
    static DisplayWidget *getInstance();

private slots:
	void sceneChecked();
	void physicsChecked();
	void tilesChecked();
	void currentModeChanged(QString mode);
};

#endif