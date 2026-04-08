#pragma once

#include <cstdint>
#include "./Vector3.h"

class GamePlayer
{
public:
	void* brain; //0x0000
	char pad_0008[48]; //0x0008
	void* minifigsList; //0x0038
	char pad_0040[4]; //0x0040
	float yAxis; //0x0044
	float zAxis; //0x0048
	float xAxis; //0x004C
	char pad_0050[116]; //0x0050
	float timeSinceLastAnim; //0x00C4
	char pad_00C8[6312]; //0x00C8
	float zRelated; //0x1970
	char pad_1974[500]; //0x1974
	float yVelocity; //0x1B68
	float zVelocity; //0x1B6C
	float xVelocity; //0x1B70
	char pad_1B74[52]; //0x1B74
	float zRelated2; //0x1BA8
	char pad_1BAC[212]; //0x1BAC

}; //Size: 0x1C80

class Player
{
private:
	GamePlayer* ptr;

public:
	Player(uintptr_t address);

	void SetVelocity(Vector3 velocity);
	Vector3 GetVelocity();
};