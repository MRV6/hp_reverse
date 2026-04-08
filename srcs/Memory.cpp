#include "../includes/Memory.h"

uintptr_t Memory::baseAddress = 0;

void Memory::Init()
{
	Memory::baseAddress = (uintptr_t)GetModuleHandle(NULL);
}

uintptr_t Memory::GetPointerAddress(uintptr_t baseOffset, const std::vector<uintptr_t> &offsets)
{
	uintptr_t address = Memory::baseAddress + baseOffset;

	__try {
		for (size_t i = 0; i < offsets.size(); i++)
		{
			address = *(uintptr_t*)address;
			address += offsets[i];
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return 0;
	}

	return address;
}