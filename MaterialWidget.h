#ifndef MATERIALWIDGET_H
#define MATERIALWIDGET_H

#include <QtGUI>
#include <string>
#include "MaterialViewWidget.h"

class Material;

class MaterialWidget : public QWidget
{
	Q_OBJECT

private:
	MaterialWidget(QGLWidget *glWidget);
	static MaterialWidget* pInstance;

	QComboBox *matCmb,*textureCmb,*normalCmb;
	QPushButton *newMaterialBtn, *colorBtn, *glowBtn, *saveBtn;
	QLabel *nameLbl,*colorLbl,*specLbl,*shineLbl,*glowLbl,
			*colorViewLbl,*glowViewLbl,*textureLbl,
			*uvOffLbl,*uvScaleLbl,*texRotateLbl,*normalLbl;
	QLineEdit *nameEdt,*uTexOffEdt,*vTexOffEdt,*uTexScaleEdt,*vTexScaleEdt;
	QSlider *specSld,*shineSld,*texRotateSld;
	QCheckBox *normalChk;

	MaterialViewWidget *materialView;

	Material *getActiveMaterial();

public:
    static MaterialWidget *getInstance(QGLWidget *glWidget = 0);

public slots:
	void refresh(string mat);

private slots:
	void currentMatChanged(QString mat);
	void saveAll();
	void addNewMaterial();
	void nameChanged(QString name);
	void specChanged(int value);
	void shineChanged(int value);
	void colorChanged();
	void glowChanged();
	void currentTexChanged(QString tex);
	void currentNormChanged(QString tex);
	void uTexOffChanged(QString value);
	void vTexOffChanged(QString value);
	void uTexScaleChanged(QString value);
	void vTexScaleChanged(QString value);
	void texRotateChanged(int value);
	void normalChanged();
};

#endif