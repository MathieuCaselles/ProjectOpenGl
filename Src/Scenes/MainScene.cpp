#include <imgui.h>
#include "MainScene.h"
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

#include "imgui.h"
#include "imgui-SFML.h"
#include "Src/ImGui/helper.h"

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

    if (!m_customSandHeight) {
        m_sandHeight = m_waterHeight + 1.5f;
    }
    if (!m_customWaterSize) {
        m_waterSize = m_terrainSize;
    }

    p_terrain->setSeed(m_seed);
    p_terrain->setAmplitude(m_amplitude);
    p_terrain->setPersistance(m_persistance);
    p_terrain->setFrequency(m_frequency);
    p_terrain->setLacunarity(m_lacunarity);
    p_terrain->setOctave(m_octave);
    p_terrain->setExponent(m_exponent);
    p_terrain->setExponent(m_exponent);
    p_terrain->setTerrainSize(m_terrainSize);
    p_terrain->setProbabilityOfALakeAppearing(m_probabilityOfALakeAppearing);
    p_terrain->setMaximumLakeRadius(m_maximumLakeRadius);
    p_terrain->setMinimumLakeRadius(m_minimumLakeRadius);
    p_terrain->setMaximumLakeDepth(m_maximumLakeDepth);
    p_terrain->setMinimumLakeDepth(m_minimumLakeDepth);



    switch (m_drawMode) {
        case 0:
            p_terrain->showPoint();
            break;
        case 1:
            p_terrain->showPolygon();
            break;
        default:
            p_terrain->showFill();
        break;
    }

    p_terrain->setSnowHeight(m_snowHeight);
    p_terrain->setStoneAngle(m_stoneAngle);
    p_terrain->setSandHeight(m_sandHeight);

    p_terrain->setLakeHeight(m_waterHeight);
    p_water->setWaterHeight(m_waterHeight);
    p_water->setWaterClearness(m_waterClearness);


    p_water->setWaterSize(m_waterSize);

    p_terrain->update(deltaTime);
    p_water->update();

    m_elapsedTimeSinceLastSecond += deltaTime;
    if (m_elapsedTimeSinceLastSecond >= 1.f) {
        m_elapsedTimeSinceLastSecond = 0;
        m_framesPerSeconds = m_numberOfFramesSinceLastSecond;
        m_numberOfFramesSinceLastSecond = 0;
    } else {
        m_numberOfFramesSinceLastSecond++;
    }
}

void MainScene::render()
{
	p_terrain->render(V, P);
	p_water->render(V, P);
}

void MainScene::createUI() {
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, (ImVec4)ImColor(m_themeColor[0], m_themeColor[1], m_themeColor[2]));
    ImGui::PushStyleColor(ImGuiCol_FrameBg, (ImVec4)ImColor(m_themeColor[0], m_themeColor[1], m_themeColor[2]));
    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.2f), getTintFromColor(m_themeColor[1], 1.2f), getTintFromColor(m_themeColor[2], 1.2f)));
    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.2f), getTintFromColor(m_themeColor[1], 1.2f), getTintFromColor(m_themeColor[2], 1.2f)));
    ImGui::PushStyleColor(ImGuiCol_CheckMark, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.8f), getTintFromColor(m_themeColor[1], 1.8f), getTintFromColor(m_themeColor[2], 1.8f)));
    ImGui::PushStyleColor(ImGuiCol_SliderGrab, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.5f), getTintFromColor(m_themeColor[1], 1.5f), getTintFromColor(m_themeColor[2], 1.5f)));
    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.6f), getTintFromColor(m_themeColor[1], 1.6f), getTintFromColor(m_themeColor[2], 1.6f)));
    ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(m_themeColor[0], m_themeColor[1], m_themeColor[2]));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.2f), getTintFromColor(m_themeColor[1], 1.2f), getTintFromColor(m_themeColor[2], 1.2f)));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor(getTintFromColor(m_themeColor[0], 1.2f), getTintFromColor(m_themeColor[1], 1.2f), getTintFromColor(m_themeColor[2], 1.2f)));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowMinSize, {400.f, 200.f});
    ImGui::Begin("Procedural generation configuration");
    ImGui::PopStyleVar();

    ImGui::SeparatorText("Terrain");
    ImGui::SliderInt("Terrain size", &m_terrainSize, 1, 2500);
    ImGui::Checkbox("Custom water size", &m_customWaterSize);
    if (!m_customWaterSize) {
        ImGui::BeginDisabled();
    }
    ImGui::SliderInt("Water size", &m_waterSize, 1, 2500);
    if (!m_customWaterSize) {
        ImGui::EndDisabled();
    }
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Should be the same as terrain size, but you can try to play with it.");
    ImGui::SliderFloat("Water clearness", &m_waterClearness, 0.f, 1.f);
    ImGui::RadioButton("Draw points", &m_drawMode, 0);
    ImGui::SameLine(); ImGui::RadioButton("Draw polygons", &m_drawMode, 1);
    ImGui::SameLine(); ImGui::RadioButton("Draw filled", &m_drawMode, 2);

    ImGui::SeparatorText("Perlin noise");
    ImGui::InputInt("Seed", &m_seed);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the seed. Same seed will always produce the same map.");
    ImGui::SliderFloat("Frequency", &m_frequency, 0.001f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes sampling frequency.");
    ImGui::SliderFloat("Lacunarity", &m_lacunarity, 0.001f, 2.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes lacunarity.");
    ImGui::SliderFloat("Amplitude", &m_amplitude, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the maximum height amplitude.");
    ImGui::SliderFloat("Persistence", &m_persistance, 0.f, 2.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the persistence.");
    ImGui::SliderInt("Octave", &m_octave, 1, 8);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the number of octaves used to generate the final perlin noise.");
    ImGui::SliderFloat("Exponent", &m_exponent, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Applies the exponent on redistribution.");
    ImGui::SliderFloat("Probability Of A Lake Appearing", &m_probabilityOfALakeAppearing, 0.f, 0.001f, "%.8f");
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes number of lakes in the map");
    ImGui::SliderInt("Maximum Lake Radius", &m_maximumLakeRadius, 5, 150);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the maximum radius of lakes");
    ImGui::SliderInt("Minimum Lake Radius", &m_minimumLakeRadius, 5, 150);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the maximum radius of lakes");
    ImGui::SliderInt("Maximum Lake Depth", &m_maximumLakeDepth, 5, 150);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the maximum depth of lakes");
    ImGui::SliderInt("Minimum Lake Depth", &m_minimumLakeDepth, 5, 150);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Changes the maximum depth of lakes");



    ImGui::SeparatorText("Additional configuration");
    ImGui::SliderFloat("Snow height", &m_snowHeight, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Defines when to start rendering snow.");
    ImGui::SliderFloat("Stone angle", &m_stoneAngle, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Defines when to start rendering stone instead of grass based on angle of vertex.");
    ImGui::SliderFloat("Water height", &m_waterHeight, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Defines when to start rendering water.");
    ImGui::Checkbox("Custom sand height", &m_customSandHeight);
    if (!m_customSandHeight) {
        ImGui::BeginDisabled();
    }
    ImGui::SliderFloat("Sand height", &m_sandHeight, 0.f, 100.f);
    ImGui::SameLine(); ImGui::Helpers::HelpMarker("Defines when to start rendering sand. Based on water if not custom.");
    if (!m_customSandHeight) {
        ImGui::EndDisabled();
    }
    ImGui::End();

    ImGui::Begin("Theme");
    ImGui::ColorPicker3("Color", &m_themeColor[0], ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
    ImGui::End();

    ImGui::Begin("Performance");
    ImGui::Text("%i FPS", m_framesPerSeconds);
    ImGui::Text("%i Terrain Primitives", (unsigned int) p_terrain->getPrimitivesCount());
    ImGui::Text("%i Water Primitives", (unsigned int) p_water->getPrimitivesCount());
    ImGui::End();

    ImGui::PopStyleColor(10);
}
