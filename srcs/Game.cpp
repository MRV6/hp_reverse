#include "../includes/Game.h"
#include "../includes/Memory.h"

Player Game::localPlayer = 0;
bool Game::localPlayerCached = false;

Player& Game::GetLocalPlayer()
{
	if (Game::localPlayerCached)
	{
		return Game::localPlayer;
	}

	uintptr_t localPlayerPtrAddress = Memory::GetPointerAddress(0x01426D50, { 0x0 });
	Player localPlayer = Player(*(uintptr_t*)localPlayerPtrAddress);

	Game::localPlayer = localPlayer;
	Game::localPlayerCached = true;

	return localPlayer;
}