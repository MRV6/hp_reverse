#include <windows.h>

#include "../includes/Menu.h"
#include "../includes/Entity.h"
#include "../includes/Model.h"
#include "../includes/Logs.h"

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

    Logs::RenderMenu();

    ImGui::Begin("HP Reverse");
    
    Entity::RenderMenu();
    Model::RenderMenu();

    ImGui::End();
}