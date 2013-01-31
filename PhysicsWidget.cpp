#include "PhysicsWidget.h"
#include "PhysicsShape.h"
#include "SceneManager.h"

PhysicsWidget* PhysicsWidget::m_pInstance = 0;

PhysicsWidget::PhysicsWidget() 
	: QWidget(0)
{

	physicsLbl = new QLabel("Physics:");
	physicsLbl->setFixedHeight(28);
	addBoxBtn = new QPushButton("Add Box");
	addSphereBtn = new QPushButton("Add Sphere");
	addCylinderBtn = new QPushButton("Add Cylinder");

	QVBoxLayout *mainLayout = new QVBoxLayout;

	mainLayout->addWidget(physicsLbl);
	mainLayout->addWidget(addBoxBtn);
	mainLayout->addWidget(addSphereBtn);
	mainLayout->addWidget(addCylinderBtn);

	setLayout(mainLayout);
	layout()->setContentsMargins(5,0,0,0);

	connect(addBoxBtn, SIGNAL(clicked()),this, SLOT(addBox()));
	connect(addSphereBtn, SIGNAL(clicked()),this, SLOT(addSphere()));
	connect(addCylinderBtn, SIGNAL(clicked()),this, SLOT(addCylinder()));

	setFixedHeight(sizeHint().height());
	setFixedWidth(150);
}

PhysicsWidget *PhysicsWidget::getInstance() {
	if (m_pInstance == 0) {
		m_pInstance = new PhysicsWidget();
	}
	return m_pInstance;
}

void PhysicsWidget::addBox()
{
	PhysicsShape *shape = new PhysicsShape(BOX);
	shape->setModel(new string("Cube"));
	shape->setMaterial(new string("Physics"));
	SceneManager::getInstance()->addPhysicsShape(shape);
}

void PhysicsWidget::addSphere()
{
	PhysicsShape *shape = new PhysicsShape(SPHERE);
	shape->setModel(new string("Sphere"));
	shape->setMaterial(new string("Physics"));
	SceneManager::getInstance()->addPhysicsShape(shape);
}

void PhysicsWidget::addCylinder()
{
	PhysicsShape *shape = new PhysicsShape(CYLINDER);
	shape->setModel(new string("Cylinder"));
	shape->setMaterial(new string("Physics"));
	SceneManager::getInstance()->addPhysicsShape(shape);
}