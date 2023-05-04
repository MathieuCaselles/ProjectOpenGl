#include <SFML/Graphics/RenderWindow.hpp>
#include <imgui-SFML.h>
#include "imgui_impl_opengl3.h"

namespace ImGui::SFML::OpenGL3 {
    void Update(sf::RenderWindow& window, sf::Time dt) {
        ImGui::SFML::Update(window, dt);
        ImGui_ImplOpenGL3_NewFrame();
    }

    void Render(sf::RenderTarget& target) {
        //target.resetGLStates();
        target.pushGLStates();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        target.popGLStates();
    }

    void Render(sf::RenderWindow& window) {
        ImGui::SFML::SetCurrentWindow(window);
        ImGui::SFML::OpenGL3::Render(static_cast<sf::RenderTarget&>(window));
    }

    void Render() {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}