#include <imgui-SFML.h>
#include "Scene.h"
#include "../ImGui/imgui_impl_opengl3_render.cpp"

#include "../Game/Game.h"

namespace Engine {

	IScene::IScene() : m_window(*GameInstance::GetInstance()->getWindow())
	{
	}

	IScene::~IScene()
	{
	}


	const sf::Vector2i IScene::getMousePositionScreen()
	{
		return sf::Mouse::getPosition();
	}

	const sf::Vector2i IScene::getMousePositionWindow()
	{
		return sf::Mouse::getPosition(m_window);
	}

	const sf::Vector2f IScene::getMousePositionView()
	{
		return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
	}

	void IScene::onBeginPlay() {
	}


	void IScene::onEndPlay() {
	}


	void IScene::processInput(sf::Event& inputEvent)
	{
	}

	void IScene::update(const float& deltaTime)
	{
        ImGui::SFML::OpenGL3::Update(m_window, sf::seconds(deltaTime));
	}

	void IScene::render()
	{
	}

	sf::RenderWindow& IScene::getWindow()
	{
		return m_window;
	}

	void IScene::renderUI() {
		createUI();
		ImGui::SFML::OpenGL3::Render(m_window);
	}

	void IScene::createUI() {
	}
}
