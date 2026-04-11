#include "../includes/Game.h"
#include "../includes/Memory.h"

Entity Game::localEntity = 0;
bool Game::localEntityCached = false;

Entity& Game::GetLocalEntity()
{
	if (Game::localEntityCached)
	{
		return Game::localEntity;
	}

	uintptr_t localEntityPtrAddr = Memory::GetPointerAddress(0x01426D50, { 0x0 });
	Entity localEntity = Entity(*(uintptr_t*)localEntityPtrAddr);

	Game::localEntity = localEntity;
	Game::localEntityCached = true;

	return localEntity;
}