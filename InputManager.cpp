#include "InputManager.h"

InputManager* InputManager::m_pInstance = 0;

InputManager *InputManager::getInstance() 
{
	if (m_pInstance == 0) 
	{
		m_pInstance = new InputManager();
	}
	return m_pInstance;
}

InputManager::InputManager()
{
	for (int i=0; i<256; i++) 
	{
		m_bKeysPressedNotChecked[i]=false;
		m_bKeys[i]=false;
	}
	for (int i=0; i<5; i++)
	{
		m_bMouseButtons[i] = false;
	}
	m_nMouseX = 0;
	m_nMouseY = 0;
	m_bMouseCentered = false;
}

void InputManager::registerKeyDown(int nKey)
{
	if (nKey < 256) 
	{
		m_bKeys[nKey]=true;
		m_bKeysPressedNotChecked[nKey]=true;
	} 
	else 
	{
		m_hsSpecialKeyPressed.insert(nKey);
	}
}

void InputManager::registerKeyUp(int nKey)
{
	if (nKey < 256) 
	{
		m_bKeys[nKey]=false;
	}
	else 
	{
		m_hsSpecialKeyPressed.erase(nKey);
	}
}

bool InputManager::isKeyDown(int nKey)
{
	m_bKeysPressedNotChecked[nKey]=false; 
	return m_bKeys[nKey];
}

bool InputManager::isSpecialKeyDown(int nKey)
{
	if (m_hsSpecialKeyPressed.find(nKey) == m_hsSpecialKeyPressed.end()) 
	{
		return false;
	}
	return true;
}

bool InputManager::isKeyDownOnce(int nKey)
{
	if(m_bKeysPressedNotChecked[nKey])
	{
		m_bKeysPressedNotChecked[nKey]=false;
		return true;
	}
	return false;
}

void InputManager::registerMouseButtonDown(int nMouse)
{
	m_bMouseButtons[nMouse] = true;
}

void InputManager::registerMouseButtonUp(int nMouse)
{
	m_bMouseButtons[nMouse] = false;
}

void InputManager::setMousePosition(int nPosX, int nPosY)
{
	m_nMouseX = nPosX;
	m_nMouseY = nPosY;
}

void InputManager::setMouseCentered(int nPosX, int nPosY)
{
	m_bMouseCentered = true;
}

bool InputManager::isMouseButtonDown(int nMouse) 
{
	return m_bMouseButtons[nMouse];
}

int InputManager::getMouseX() 
{
	return m_nMouseX;
}

int InputManager::getMouseY() 
{
	return m_nMouseY;
}

bool InputManager::isMouseMoved() 
{
	return m_bMouseMoved;
}