#include "Game.h"
#include <cassert>
#include "../Scene/Scene.h"
#include "Tools/MathUtils.h"
#include <GL/glew.h>
#include<SFML/OpenGL.hpp>
#include "imgui.h"
#include "imgui-SFML.h"

namespace Engine {


Game::~Game()
{
    clearScenes();
}

void Game::run(sf::VideoMode videoMode, std::string windowTitle, sf::Uint32 style, const size_t indexStartScene)
{
    m_pCurrentScene = m_scenes.at(indexStartScene);

    assert(m_pCurrentScene != nullptr);

    initWindow(videoMode, windowTitle, style);
    glEnable(GL_DEPTH_TEST);

    // fucking lines of hell
    glewExperimental = GL_TRUE;
    if (glewInit())
        throw std::runtime_error("Error");

    ImGui::SFML::Init(m_window);

    m_pCurrentScene->onBeginPlay();

    sf::Clock DeltaTimeClock;

    while (m_window.isOpen()) {
        sf::Time deltaTime = DeltaTimeClock.restart();
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        processInput();
        update(deltaTime.asSeconds(), deltaTime);
        render();
    }
    ImGui::SFML::Shutdown();
}

sf::RenderWindow* Game::getWindow()
{
    return &m_window;
}



void Game::setCurrentScene(const size_t index)
{
    m_pCurrentScene->onEndPlay();
    m_pCurrentScene = m_scenes.at(index);
    m_pCurrentScene->onBeginPlay();

}

void  Game::deleteSceneByIndex(const size_t index)
{
    m_scenes.erase(m_scenes.begin() + index);
}


void Game::clearScenes()
{
    for (IScene* pScene : m_scenes)
    {
        delete pScene;
    }
    m_scenes.clear();
}



// private
Game::Game()
    : m_window(sf::RenderWindow()), m_pCurrentScene(nullptr)
{
}


void Game::initWindow(sf::VideoMode videoMode, std::string windowTitle, sf::Uint32 style, const sf::ContextSettings settings)
{
    m_window.create(videoMode, windowTitle, style, settings);
    m_window.setVerticalSyncEnabled(true);
    m_window.setActive(true);

}

void Game::processInput()
{

    //TODO: signal pattern instead of this
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window.close();
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
        }

        m_pCurrentScene->processInput(event);
    }
}

void Game::update(const float& deltaTime, sf::Time clockRestart)
{
    m_pCurrentScene->update(deltaTime, clockRestart);
}

void Game::render()
{
    // clear the buffers
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_window.clear();

    m_pCurrentScene->render();

    glFlush();
    m_window.display();
}


}