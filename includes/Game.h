#pragma once

#include "./Player.h"

class Game
{
private:
	static Player localPlayer;
	static bool localPlayerCached;

public:
	static Player& GetLocalPlayer();
};