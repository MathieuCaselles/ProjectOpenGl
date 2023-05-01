#include "MainScene.h"
#include "GL/glew.h"
#include "SFML/OpenGL.hpp"

using Point3f = Tools::Point3d<float>;
using Mat4f = Tools::Mat4<float>;
using Terrainf =Terrain<float>;
using Waterf = Water<float>;

MainScene::MainScene(): m_aspect(800.f / 600.f), m_fov(45.f / 180.f * Tools::PI), m_n(0.1f), m_f(2000.f), m_cameraPos(0.f, 50.f, 0.f), m_cameraAlpha(-2.50f), m_cameraBeta(0.25f), m_cameraSpeed(1.f)
{
}

MainScene::~MainScene()
{
}

void MainScene::onBeginPlay() {
	sf::Mouse::setPosition(sf::Vector2i(400, 300), m_window);

    terrain = std::make_unique<Terrainf>();
    water = std::make_unique<Waterf>();
}

void MainScene::processInput(sf::Event& inputEvent)
{
    if (inputEvent.key.code == sf::Keyboard::Escape) {
        m_window.close();
    }
    else if (inputEvent.type == sf::Event::MouseMoved) {
        float dx = 400.f - float(inputEvent.mouseMove.x);
        float dy = 300.f - float(inputEvent.mouseMove.y);
        sf::Mouse::setPosition(sf::Vector2i(400, 300), m_window);
        m_cameraAlpha += 0.001f * dx;
        m_cameraBeta -= 0.001f * dy;
    }

    printf("Camera position: %f, %f\n", m_cameraAlpha, m_cameraBeta);
		
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
        m_cameraPos.y += m_cameraSpeed; m_cameraSpeed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_cameraPos.y -= m_cameraSpeed;
    }
    

	IScene::processInput(inputEvent);
}

void MainScene::update(const float& deltaTime)
{
    V = Mat4f::rotationX(-m_cameraBeta) * Mat4f::rotationY(-m_cameraAlpha) * Mat4f::translation(-m_cameraPos.x, -m_cameraPos.y, -m_cameraPos.z);

    terrain->update();
    water->update();
}

void MainScene::render()
{
    terrain->render(V, P);
    water->render(V, P);

    glFlush();
    //Ui render i guess
}