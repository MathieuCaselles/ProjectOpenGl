#include <imgui.h>
#include "MainScene.h"
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

using Point3f = Tools::Point3d<float>;
using Mat4f = Tools::Mat4<float>;
using Terrainf = Terrain<float>;
using Waterf = Water<float>;
sf::Clock deltaClock;

MainScene::MainScene() : m_aspect(800.f / 600.f), m_fov(45.f / 180.f * Tools::PI), m_n(0.1f), m_f(2000.f), m_cameraPos(0.f, 50.f, 0.f), m_cameraAlpha(-2.50f), m_cameraBeta(0.25f), m_cameraSpeed(1.f)
{
}

MainScene::~MainScene()
{
}

void MainScene::onBeginPlay() {
	sf::Mouse::setPosition(sf::Vector2i(m_window.getSize().x / 2, m_window.getSize().y / 2), m_window);
	p_terrain = std::make_unique<Terrainf>();
	p_water = std::make_unique<Waterf>();
}

void MainScene::processInput(sf::Event& inputEvent)
{
	if (inputEvent.key.code == sf::Keyboard::Escape) {
        m_window.close();
        return;
    }

	m_window.setMouseCursorVisible(true);
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		m_window.setMouseCursorVisible(false);

        const auto size = m_window.getSize();
        const auto halfX = (size.x / 2);
        const auto halfY = (size.y / 2);
		sf::Mouse::setPosition(sf::Vector2i(halfX, halfY), m_window);

		if (inputEvent.type == sf::Event::MouseMoved && m_mouseLocked == true) {
			float dx = halfX - float(inputEvent.mouseMove.x);
			float dy = halfY - float(inputEvent.mouseMove.y);
			m_cameraAlpha += 0.001f * dx;
			m_cameraBeta -= 0.001f * dy;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			m_cameraPos.x -= sin(m_cameraAlpha) * m_cameraSpeed;
			m_cameraPos.z -= cos(m_cameraAlpha) * m_cameraSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
		{
			m_cameraPos.x -= cos(m_cameraAlpha) * m_cameraSpeed;
			m_cameraPos.z += sin(m_cameraAlpha) * m_cameraSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			m_cameraPos.x += sin(m_cameraAlpha) * m_cameraSpeed;
			m_cameraPos.z += cos(m_cameraAlpha) * m_cameraSpeed;

		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			m_cameraPos.x += cos(m_cameraAlpha) * m_cameraSpeed;
			m_cameraPos.z -= sin(m_cameraAlpha) * m_cameraSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
		{
			m_cameraPos.y += m_cameraSpeed;
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			m_cameraPos.y -= m_cameraSpeed;
		}

	}

	IScene::processInput(inputEvent);
}

void MainScene::update(const float& deltaTime)
{
    IScene::update(deltaTime);
	V = Mat4f::rotationX(-m_cameraBeta) * Mat4f::rotationY(-m_cameraAlpha) * Mat4f::translation(-m_cameraPos.x, -m_cameraPos.y, -m_cameraPos.z);

    p_terrain->setSeed(m_seed);
    p_terrain->setFrequency(m_frequency);
    p_terrain->setAmplitude(m_amplitude);
    p_terrain->setOctave(m_octave);
    p_terrain->setExponent(m_exponent);

	p_terrain->update();
	p_water->update();
}

void MainScene::render()
{
	p_terrain->render(V, P);
	p_water->render(V, P);
}

void MainScene::createUI() {
    IScene::createUI();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {200.f, 300.f});
    ImGui::Begin("Procedural generation configuration", nullptr, ImGuiWindowFlags_NoCollapse );
    ImGui::PopStyleVar();
    ImGui::InputInt("Seed", &m_seed);
    ImGui::InputFloat("Frequency", &m_frequency);
    ImGui::InputFloat("Amplitude", &m_amplitude);
    ImGui::InputInt("Octave", &m_octave);
    ImGui::InputFloat("Exponent", &m_exponent);
    ImGui::End();
}
