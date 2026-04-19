#pragma once

#include <cstdint>

struct Offsets
{
	// Game functions
	static constexpr uintptr_t fnSpawnEntity = 0x25E550;
	static constexpr uintptr_t fnSetEntityModel = 0x2B61E0;
	static constexpr uintptr_t fnTeleportEntity = 0x33CC0;
	static constexpr uintptr_t fnKillEntity = 0x29DE40;
	static constexpr uintptr_t fnLoadModel = 0x2A8190;
	static constexpr uintptr_t fnGetLoadedCharDefFile = 0x527F0;

	// Values
	static constexpr uintptr_t entityList = 0x01426D50;
	static constexpr uintptr_t localEntity = 0x01426D50; // TODO: Use the real local entity and not the first entity (which is ours if we don't switch)
	static constexpr uintptr_t levelPtrParent = 0x1420130;
	static constexpr uintptr_t levelOffset = 0x268;
	static constexpr uintptr_t world = 0x141B3A0;
	static constexpr uintptr_t modelsList = 0x1417608;
	static constexpr uintptr_t modelsCountOffset = 0x1417610;
	static constexpr uintptr_t studs = 0x01419430;

	static constexpr uintptr_t gameLoopHook = 0x52BED0;

};