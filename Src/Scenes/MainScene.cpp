#include "MainScene.h"

MainScene::MainScene()
{

}

MainScene::~MainScene()
{
}

void MainScene::processInput(sf::Event& inputEvent)
{
	if (inputEvent.key.code == sf::Keyboard::Escape)
		m_window.close();

	IScene::processInput(inputEvent);
}