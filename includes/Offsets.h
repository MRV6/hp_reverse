#pragma once

#include <cstdint>

struct Offsets
{
	// Values
	static constexpr uintptr_t entityList = 0x01426D50;

	static constexpr uintptr_t localEntity = 0x1419800;

	static constexpr uintptr_t levelPtrParent = 0x1420130;
	static constexpr uintptr_t levelOffset = 0x268;

	static constexpr uintptr_t world = 0x141B3A0;

	static constexpr uintptr_t modelsList = 0x1417608;
	static constexpr uintptr_t modelsCountOffset = 0x1417610;

	static constexpr uintptr_t studs = 0x01419430;

	static constexpr uintptr_t currentLevelOffset = 0x1420130;

	static constexpr uintptr_t gameLoopHook = 0x52BED0;
};