#pragma once

#include "../vendor/imgui/imgui.h"

class Menu
{
private:
	static bool isVisible;

public:
	static void Loop();
	static void Render();
};