#pragma once

#include <cstdint>

class World
{
public:
	char pad_0000[8]; //0x0000
	int16_t modelsCount; //0x0008
	char pad_000A[62]; //0x000A
	void* currentLevel; //0x0048
	char pad_0050[2032]; //0x0050
}; //Size: 0x0840