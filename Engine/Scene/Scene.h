#pragma once

#include <SFML/Graphics.hpp>

namespace Engine {


    class IScene
    {

    public:
        IScene();
        virtual ~IScene();

        virtual const sf::Vector2i getMousePositionScreen();
        virtual const sf::Vector2i getMousePositionWindow();
        virtual const sf::Vector2f getMousePositionView();

        virtual void onBeginPlay();
        virtual void onEndPlay();

        virtual void processInput(sf::Event& inputEvent);
        virtual void update(const float& deltaTime, sf::Time clockRestart);
        virtual void render();

     
        sf::RenderWindow& getWindow();



    protected:
        sf::RenderWindow& m_window;

    };


}
