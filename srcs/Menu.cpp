#include <windows.h>

#include "../includes/Menu.h"
#include "../includes/Entity.h"
#include "../includes/Model.h"
#include "../includes/Logs.h"
#include "../includes/Level.h"

bool Menu::isVisible = false;

void Menu::Loop()
{
    if (GetAsyncKeyState(VK_F3) & 0x1)
    {
        Menu::isVisible = !Menu::isVisible;
    }
}

void Menu::Render()
{
    if (!Menu::isVisible)
    {
        return;
    }

    if (ImGui::BeginMainMenuBar())
    {
        if (ImGui::BeginMenu("Tools"))
        {
            ImGui::Checkbox("Logs", &Logs::menuVisible);
            ImGui::Checkbox("Entities", &Entity::menuVisible);
            ImGui::Checkbox("Models", &Model::menuVisible);
            ImGui::Checkbox("Level", &Level::menuVisible);
            ImGui::EndMenu();
        }

        ImGui::EndMainMenuBar();
    }
    
    Logs::RenderMenu();
    Entity::RenderMenu();
    Model::RenderMenu();
    Level::RenderMenu();
}