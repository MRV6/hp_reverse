#include "../includes/Level.h"
#include "../includes/Offsets.h"
#include "../includes/Memory.h"

#include "../vendor/imgui/imgui.h"

bool Level::menuVisible = false;

void Level::Init()
{

}

void Level::RenderMenu()
{
	if (!Level::menuVisible)
	{
		return;
	}

	ImGui::Begin("Level");

	uintptr_t address = *(uintptr_t*)(Memory::GetBaseAddress() + Offsets::currentLevelOffset);

	if (address != 0)
	{
		ImGui::Text("Current level: %s", (char*)address);
	}

	ImGui::End();
}