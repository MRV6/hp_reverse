#pragma once

#include <cstdint>
#include <vector>

#include "../vendor/imgui/imgui.h"

class GameModel
{
public:
	char pad_0000[16]; //0x0000
	char* label; //0x0010
	char pad_0018[8]; //0x0018
	char* path; //0x0020
	char* name; //0x0028
	char pad_0030[71424]; //0x0030
}; //Size: 0x11730

class Model
{
private:
	static uintptr_t listAddress;
	static int modelsCountOffset;
	static int modelDataSize;

	GameModel* ptr;

public:
	Model(uintptr_t address);

	static void Init();
	static void RenderMenu();

	static std::vector<Model> GetAll();
};