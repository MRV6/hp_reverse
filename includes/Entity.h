#pragma once

#include <cstdint>
#include <vector>

#include "./Vector3.h"
#include "../vendor/imgui/imgui.h"

#define ENTITY_MEM_SIZE 0x1E80

class GameEntity
{
public:
	void* brain; //0x0000
	char pad_0008[32]; //0x0008
	void* selfPtr; //0x0028
	char pad_0030[8]; //0x0030
	void* modelNamePtr; //0x0038
	char pad_0040[4]; //0x0040
	float yAxis; //0x0044
	float zAxis; //0x0048
	float xAxis; //0x004C
	char pad_0050[116]; //0x0050
	float timeSinceLastAnim; //0x00C4
	char pad_00C8[288]; //0x00C8
	void* focusedEntity; //0x01E8
	char pad_01F0[976]; //0x01F0
	void* currentTerrain; //0x05C0
	char pad_05C8[3184]; //0x05C8
	void* someEntity; //0x1238
	char pad_1240[1840]; //0x1240
	float zRelated; //0x1970
	char pad_1974[203]; //0x1974
	uint8_t health; //0x1A3F
	char pad_1A41[287]; //0x1A41
	void* minifigPtr; //0x1B60
	float yVelocity; //0x1B68
	float zVelocity; //0x1B6C
	float xVelocity; //0x1B70
	char pad_1B74[52]; //0x1B74
	float zRelated2; //0x1BA8
	char pad_1BAC[9428]; //0x1BAC
}; //Size: 0x4080

class Entity
{
private:
	GameEntity* ptr;

public:
	Entity(uintptr_t address);

	void SetCoords(Vector3 coords);
	Vector3 GetCoords() const;

	void SetVelocity(Vector3 velocity);
	Vector3 GetVelocity() const;

	std::string GetModelName() const;

	unsigned int GetHealth() const;
	void SetHealth(unsigned int health);

	uintptr_t GetAddress() const;

	static std::vector<Entity> GetAll();
	static void RenderMenu();
};

std::ostream& operator<<(std::ostream& os, const Entity& ent);