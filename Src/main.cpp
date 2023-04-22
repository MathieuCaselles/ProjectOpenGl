#include <Engine/Game/Game.h>

#include "Scenes/SceneEnum.h"
#include "Scenes/MainScene.h"
#include <ProceduralGeneration/Grid3d.h>
#include <Tools/MathUtils.h>

int main()
{
    ProceduralGeneration::Grid3d grid(10, 10, 10);

    //print all cells
    for (int x = 0; x < grid.getWidth(); ++x)
    {
        for (int y = 0; y < grid.getHeight(); ++y)
        {
            for (int z = 0; z < grid.getDepth(); ++z)
            {
				std::cout << "Cell(" << x << ", " << y << ", " << z << ") = " << grid.getCell(x, y, z).X << ", " << grid.getCell(x, y, z).Y << ", " << grid.getCell(x, y, z).Z << std::endl;
			}
		}
	}

    //Engine::Game* game = Engine::GameInstance::GetInstance();
    //game->addScenes(new MainScene());
    //game->run(sf::VideoMode(1280, 720), "ProceduralGeneration", sf::Style::Default, ScenesEnum::MAIN_SCENE);

    return 0;
}
