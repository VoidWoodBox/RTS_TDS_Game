////////////////////////////////////////////////
// Include
#include "interfaceWindow.h"

#include "camera.h"
#include "interface/uiButton.h"
#include "interface/uiText.h"
#include "interface/uiObject.h"
#include "interface/uiSprite.h"
#include "eventSystem/eventController.h"
#include "tools/utils.h"
#include "dataManager.h"

#include <boost\foreach.hpp>

#include <string>

////////////////////////////////////////////////
// Extern
extern sf::Font* g_font;

////////////////////////////////////////////////
// Class InterfaceWindow
InterfaceWindow::InterfaceWindow()
{
	m_isActive = false;
	m_dataManager = DataManager::instance();
}

InterfaceWindow::~InterfaceWindow()
{
	// Delete interface objects
	for each (UIObject* obj in m_objectsList)
	{
		if (obj != 0)
		{
			delete obj;
			obj = 0;
		}
	}

	m_objectsList.clear();
}

void InterfaceWindow::createButtons()
{
	PropertyTree buttons = m_objects.get_child("Buttons");
	int x, y, width, height;

	BOOST_FOREACH(auto &v, buttons)
	{
		UIButton* button = new UIButton();
		button->setName(v.first);
		button->setCamera(m_camera);
		button->initialize(v.second);

		m_objectsList.push_back(button);
	}
}

void InterfaceWindow::createTexts()
{
	PropertyTree texts = m_objects.get_child("Texts");
	int x, y;

	BOOST_FOREACH(auto &v, texts)
	{
		UIText* text = new UIText();
		text->setName(v.first);
		text->setCamera(m_camera);
		text->initialize(v.second);

		m_objectsList.push_back(text);
	}
}

void InterfaceWindow::createSprites()
{

}

void InterfaceWindow::initialize(std::string name)
{
	m_objects = m_dataManager->getInterfaceSettings()->get_child(name);

	createButtons();
	createTexts();
}


void InterfaceWindow::update()
{
	sf::Vector2i mousePosition = sf::Mouse::getPosition(*m_camera->getRenderWindow());
	sf::Vector2f worldPosition = m_camera->getRenderWindow()->mapPixelToCoords(mousePosition);

	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] != 0)
			m_objectsList[i]->update();
	}
	
	if (IsKeyDown(0x70) && m_eventController->getEventState(GameEvent_LeftButtonStay))
	{
		for (int i = 0; i < 30; i++)
		{
			if (m_objectsList[i] != 0 && m_objectsList[i]->getMouseOnObject() && m_eventController->getEventState(GameEvent_LeftButtonStay) )
			{
				// Calculate cursor position on window
				Vec2 windowCursorPosition = Vec2(round(worldPosition.x), round(worldPosition.y)) + m_camera->getPosition();
				m_objectsList[i]->setPosition(windowCursorPosition);

				break;
			}
		}
	}
}

void InterfaceWindow::render()
{
	for (int i = 0; i < 30; i++)
	{
		if (m_objectsList[i] != 0)
		{
			m_objectsList[i]->render();
		}
	}
}