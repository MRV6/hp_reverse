#pragma once

#include <vector>
#include <string>

#include "../../vendor/imgui/imgui.h"

class Logs
{
private:
	static ImGuiTextBuffer logs;

public:
	static bool menuVisible;

	static void RenderMenu();
	static void Add(const char* fmt, ...) IM_FMTARGS(2);
};