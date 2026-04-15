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
	uint64_t unkFlags; //0x0008
	char pad_0010[40]; //0x0010
	void* modelNamePtr; //0x0038
	char pad_0040[4]; //0x0040
	float yAxis; //0x0044
	float zAxis; //0x0048
	float xAxis; //0x004C
	float yAxis3; //0x0050
	float zAxis3; //0x0054
	float xAxis3; //0x0058
	float yAxis4; //0x005C
	float zAxis4; //0x0060
	float xAxis4; //0x0064
	char pad_0068[384]; //0x0068
	void* focusedEntity; //0x01E8
	char pad_01F0[976]; //0x01F0
	void* currentTerrain; //0x05C0
	char pad_05C8[3184]; //0x05C8
	void* someEntity; //0x1238
	char pad_1240[1840]; //0x1240
	float zRelated; //0x1970
	char pad_1974[166]; //0x1974
	uint16_t modelIndex; //0x1A1A
	char pad_1A1C[34]; //0x1A1C
	uint8_t maxHealth; //0x1A3E
	uint8_t health; //0x1A3F
	char pad_1A40[280]; //0x1A40
	void* charDefFilePtr; //0x1B58
	void* minifigPtr; //0x1B60
	float yVelocity; //0x1B68
	float zVelocity; //0x1B6C
	float xVelocity; //0x1B70
	char pad_1B74[24]; //0x1B74
	float yAxis2; //0x1B8C
	float zAxis2; //0x1B90
	float xAxis2; //0x1B94
	char pad_1B98[16]; //0x1B98
	float zRelated2; //0x1BA8
	char pad_1BAC[724]; //0x1BAC
}; //Size: 0x1E80

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

	unsigned int GetMaxHealth() const;
	void SetMaxHealth(unsigned int maxHealth);

	void ResetHealth();

	uintptr_t GetAddress() const;

	unsigned int GetModel() const;

	static std::vector<Entity> GetAll();
	static void RenderMenu();
};

std::ostream& operator<<(std::ostream& os, const Entity& ent);