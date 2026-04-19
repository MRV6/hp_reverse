#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <functional>

#include "../vendor/imgui/imgui.h"
#include "./World.h"

class GameModel
{
public:
	char pad_0000[8]; //0x0000
	uint16_t modelIndex; //0x0008
	char pad_000A[6]; //0x000A
	char* label; //0x0010
	char pad_0018[8]; //0x0018
	char* path; //0x0020
	char* name; //0x0028
	char pad_0030[71424]; //0x0030
}; //Size: 0x11730

class CharDefFile
{
public:
	char pad_0000[24]; //0x0000
	uint16_t modelIndex; //0x0018
	char pad_001A[4148]; //0x001A
}; //Size: 0x104E

class Model
{
private:
	static uintptr_t listAddress;
	static int modelDataSize;

	GameModel* ptr;

public:
	Model(uintptr_t address);

	static void Init();
	static void RenderMenu();
	static std::vector<Model> GetAll();
	static bool showLoadedOnly;
	static bool sortListByIndex;

	char* GetName() const;
	char* GetLabel() const;
	char* GetPath() const;
	unsigned int GetModelIndex() const;

	bool IsLoaded() const;
	void Load() const;

	std::string GetType() const;
};

typedef uintptr_t (__fastcall* _getLoadedCharDefFile)(World* world, uintptr_t level, unsigned int modelIndex);
typedef bool(__fastcall* _loadModel)(unsigned int modelIndex, uintptr_t level, bool a3, bool a4, uintptr_t* unk);