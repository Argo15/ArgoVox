#ifndef PHYSICSWIDGET_H
#define PHYSICSWIDGET_H

#include <QtGui>

class PhysicsWidget : public QWidget
{
	Q_OBJECT

private:
	PhysicsWidget();
	static PhysicsWidget* m_pInstance;

	QLabel *physicsLbl;
	QPushButton *addBoxBtn, *addSphereBtn, *addCylinderBtn;


public:
    static PhysicsWidget *getInstance();
	
public slots:
	void addBox();
	void addSphere();
	void addCylinder();

};

#endif