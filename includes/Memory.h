#pragma once

#include <Windows.h>
#include <vector>
#include <iostream>

class Memory
{
private:
	static uintptr_t baseAddress;

public:
	static void Init();
	static uintptr_t GetPointerAddress(uintptr_t baseOffset, const std::vector<uintptr_t> &offsets);
	static uintptr_t GetBaseAddress();
};