#include "../includes/Game.h"
#include "../includes/Memory.h"
#include "../includes/Offsets.h"

Entity Game::localEntity = 0;
bool Game::localEntityCached = false;

Entity& Game::GetLocalEntity()
{
	if (Game::localEntityCached)
	{
		return Game::localEntity;
	}

	uintptr_t localEntityPtrAddr = Memory::GetPointerAddress(Offsets::localEntity, { 0x0 });
	Entity localEntity = Entity(*(uintptr_t*)localEntityPtrAddr);

	Game::localEntity = localEntity;
	Game::localEntityCached = true;

	return localEntity;
}