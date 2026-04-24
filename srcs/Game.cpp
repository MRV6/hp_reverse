#include "../includes/Game.h"
#include "../includes/Memory.h"
#include "../includes/Offsets.h"

#include "../vendor/patterns/Hooking.Patterns.h"

GameFn::KillEntity Game::KillEntity = nullptr;
GameFn::SpawnEntity Game::SpawnEntity = nullptr;
GameFn::SetEntityModel Game::SetEntityModel = nullptr;
GameFn::TeleportEntity Game::TeleportEntity = nullptr;
GameFn::GetLoadedCharFile Game::GetLoadedCharFile = nullptr;
GameFn::LoadModel Game::LoadModel = nullptr;
GameFn::FollowPlayer Game::FollowPlayer = nullptr;

void Game::Init()
{
	Game::KillEntity = reinterpret_cast<GameFn::KillEntity>(hook::get_pattern("48 8B C4 48 89 58 10 48 89 70 18 48 89 78 20 55 41 54 41 55 41 56 41 57 48 8D 68 C8", 0x0));

	Game::SpawnEntity = reinterpret_cast<GameFn::SpawnEntity>(hook::get_pattern("40 53 55 56 41 54 41 55 41 56 41 57 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 84 24 ? ? ? ? 4C", 0x0));
	
	Game::SetEntityModel = reinterpret_cast<GameFn::SetEntityModel>(hook::get_pattern("40 53 56 57 48 81 EC ? ? ? ? 8B FA", 0x0));
	
	Game::TeleportEntity = reinterpret_cast<GameFn::TeleportEntity>(hook::get_pattern("40 57 48 83 EC ? F2 0F 10 02", 0x0));
	
	Game::GetLoadedCharFile = reinterpret_cast<GameFn::GetLoadedCharFile>(hook::get_pattern("48 89 5C 24 ? 48 89 6C 24 ? 56 48 83 EC ? 49 63 E8", 0x0));
	
	Game::LoadModel = reinterpret_cast<GameFn::LoadModel>(hook::get_pattern("40 53 57 41 54 41 55 41 57 48 83 EC ? 48 8B 05", 0x0));

	Game::FollowPlayer = reinterpret_cast<GameFn::FollowPlayer>(hook::get_pattern("48 8B C4 53 55 56 48 83 EC", 0x0));
}