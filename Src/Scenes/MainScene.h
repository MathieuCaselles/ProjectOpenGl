#pragma once

#include <Engine/Scene/Scene.h>

class MainScene : public Engine::IScene
{
public:

    MainScene();
    ~MainScene();

    void processInput(sf::Event& inputEvent) override;
};