#include <QtGui>
#include "QtCamera.h"
#include "InputManager.h"

void QtCamera::move(float speed) {
	if (InputManager::getInstance()->isKeyDown(Qt::Key_W))
		moveForward(speed*0.1f);
	if (InputManager::getInstance()->isKeyDown(Qt::Key_S))
		moveBackward(speed*0.1f);
	if (InputManager::getInstance()->isKeyDown(Qt::Key_A))
		moveLeft(speed*0.1f);
	if (InputManager::getInstance()->isKeyDown(Qt::Key_D))
		moveRight(speed*0.1f);
	if (InputManager::getInstance()->isKeyDown(Qt::Key_E))
		moveUp(speed*0.1f);
	if (InputManager::getInstance()->isKeyDown(Qt::Key_Q))
		moveDown(speed*0.1f);

	if (InputManager::getInstance()->isMouseButtonDown(Qt::RightButton)) {
		mouseRotate();
	} else {
		m_nLastMouseX = -1;
		m_nLastMouseY = -1;
	}
}