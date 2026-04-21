#include "../includes/Logs.h"

ImGuiTextBuffer Logs::logs = {};
bool Logs::visible = false;

void Logs::Add(const char* fmt, ...) IM_FMTARGS(2)
{
	int old_size = Logs::logs.size();
	va_list args;
	va_start(args, fmt);
	Logs::logs.appendfv(fmt, args);
	va_end(args);
	Logs::logs.append("\n");
}

void Logs::RenderMenu()
{
	if (!Logs::visible)
	{
		return;
	}

	ImGui::Begin("HP Reverse : logs");

	if (ImGui::BeginChild("##logs", ImVec2(0.0f, 0.0f), ImGuiChildFlags_Borders, ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_AlwaysHorizontalScrollbar))
	{
		ImGui::TextUnformatted(Logs::logs.begin());

		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
		{
			ImGui::SetScrollHereY(1.0f);
		}

		ImGui::EndChild();
	}

	ImGui::End();
}