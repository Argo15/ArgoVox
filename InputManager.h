#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <set>

class InputManager 
{
private:
	static InputManager *m_pInstance;
	bool m_bKeys[256];
	bool m_bKeysPressedNotChecked[256];
	bool m_bMouseButtons[5];
	std::set<int> m_hsSpecialKeyPressed;
	int m_nMouseX, m_nMouseY;
	bool m_bMouseCentered;
	bool m_bMouseMoved;

	InputManager();
public:
	static InputManager *getInstance();

	void registerKeyDown(int nKey);
	void registerKeyUp(int nKey);
	bool isKeyDown(int nKey);
	bool isSpecialKeyDown(int nKey);
	bool isKeyDownOnce(int nKey);

	void registerMouseButtonDown(int nMouse);
	void registerMouseButtonUp(int nMouse);
	void setMousePosition(int nPosX, int nPosY);
	void setMouseCentered(int nPosX, int nPosY);
	bool isMouseButtonDown(int nMouse);
	int getMouseX();
	int getMouseY();
	bool isMouseMoved();
};

#endif