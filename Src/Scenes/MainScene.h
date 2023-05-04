#pragma once

#include <Engine/Scene/Scene.h>

#include "Src/Terrain/Terrain.h"
#include "Src/Terrain/Water.h"

//Aliases declaration

using Point3f = Tools::Point3d<float>;
using Mat4f = Tools::Mat4<float>;
using Terrainf = Terrain<float>;
using Waterf = Water<float>;

class MainScene : public Engine::IScene
{
public:

    MainScene();
    ~MainScene();
    
    void onBeginPlay() override;

    void processInput(sf::Event& inputEvent) override;
    void update(const float& deltaTime) override;
    void render() override;

private:

    // Camera
    Point3f m_cameraPos;
    float m_cameraAlpha; //YAW
    float m_cameraBeta; //PITCH
    float m_cameraSpeed;

    bool m_mouseLocked = true;

    Mat4f V;

    const float m_aspect;
    const float m_fov;
    const float m_n;
    const float m_f;

    Tools::Mat4<float> P = Tools::Mat4<float>::projection(m_aspect, m_fov, m_f, m_n);

    //Terrain
    std::unique_ptr<Terrainf> terrain;
    std::unique_ptr<Waterf> water;

};