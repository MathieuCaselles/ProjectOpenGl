#pragma once

#include <Engine/Scene/Scene.h>

#include "../Terrain/Terrain.h"

//Aliases declaration

using Point3f = Tools::Point3d<float>;
using Mat4f = Tools::Mat4<float>;
using Terrainf = Terrain<float>;

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

    Mat4f V;

    const float m_aspect = 800.f / 600.f;
    const float m_fov = 45.f / 180.f * 3.141592f;
    const float m_n = 0.1f;
    const float m_f = 100.f;

    Tools::Mat4<float> P = Tools::Mat4<float>::projection(m_aspect, m_fov, m_f, m_n);

    //Terrain
    std::unique_ptr<Terrainf> terrain;

};