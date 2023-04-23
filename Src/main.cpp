#include <Engine/Game/Game.h>

#include "Scenes/SceneEnum.h"
#include "Scenes/MainScene.h".
#include "Engine/Terrain/Terrain.h"

int main()
{
    const sf::ContextSettings settings(24, 8, 4, 4, 6);

    Engine::Game* game = Engine::GameInstance::GetInstance();
    game->addScenes(new MainScene());
    game->run(sf::VideoMode(1280, 720), "ProceduralGeneration", sf::Style::Default, ScenesEnum::MAIN_SCENE);
    return 0;
}
