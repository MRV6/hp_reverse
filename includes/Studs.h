#pragma once

#include <cstdint>

class Studs
{
private:
	static uintptr_t address;

public:
	static void Init();

	static void Add(unsigned int amount);
	static void Remove(unsigned int amount);

	static void Set(unsigned int amount);
	static unsigned int Get();
};