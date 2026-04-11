#pragma once

#include "./Entity.h"

class Game
{
private:
	static Entity localEntity;
	static bool localEntityCached;

public:
	static Entity& GetLocalEntity();
};