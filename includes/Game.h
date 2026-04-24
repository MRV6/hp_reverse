#pragma once

#include "./Entity.h"
#include "./World.h"

namespace GameFn
{
	typedef GameEntity* (__fastcall* SpawnEntity)(unsigned int modelIndex, float* coords, unsigned int heading, void* unkClass, void* unkClass2, bool placeToGround, void* a7, void* unkClass3, char a9, int a10, void* unkClass4, char unkFlags);
	typedef char(__fastcall* TeleportEntity)(GameEntity* entity, float* coords);
	typedef bool(__fastcall* SetEntityModel)(GameEntity* entity, unsigned int modelId, unsigned int oldModelId);
	typedef bool(__fastcall* KillEntity)(GameEntity* entity, int a1, int a2, int a3, bool a4, bool a5, int a6);
	typedef uintptr_t(__fastcall* GetLoadedCharFile)(World* world, uintptr_t level, unsigned int modelIndex);
	typedef bool(__fastcall* LoadModel)(unsigned int modelIndex, uintptr_t level, bool a3, bool a4, uintptr_t* unk);
};

class Game
{
public:
	static void Init();

	static GameFn::SpawnEntity SpawnEntity;
	static GameFn::TeleportEntity TeleportEntity;
	static GameFn::SetEntityModel SetEntityModel;
	static GameFn::KillEntity KillEntity;
	static GameFn::GetLoadedCharFile GetLoadedCharFile;
	static GameFn::LoadModel LoadModel;
};