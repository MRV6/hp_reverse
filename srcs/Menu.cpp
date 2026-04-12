#include <windows.h>

#include "../includes/Menu.h"
#include "../includes/Entity.h"

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

    ImGui::Begin("HP Reverse");
    
    Entity::RenderMenu();

    ImGui::End();
}