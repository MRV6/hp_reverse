#include "../includes/Game.h"
#include "../includes/Memory.h"
#include "../includes/Offsets.h"

Entity& Game::GetLocalEntity()
{
	uintptr_t localEntityPtr = Memory::GetBaseAddress() + Offsets::localEntity;
	Entity localEntity = Entity(*(uintptr_t*)localEntityPtr);

	return localEntity;
}