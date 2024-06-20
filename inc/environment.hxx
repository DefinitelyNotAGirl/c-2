#pragma once
#include <cstdint>
/**
 * @brief bit fields:
 * 0: pseudo unix
 * 1: true unix
 * 2: apple
 * 3: linux
 * 4: windows
 * 5-7: bit width
 * 8-63: reserved, MBZ
 */
enum class System : uint64_t {
	Apple = (1<<0) | (1<<2) | (1<<5),
	Linux = (1<<0) | (1<<1) | (1<<3) | (1<<5),
	Windows = (1<<4) | (1<<5)
};

enum class Architecture : uint64_t {
	AMD64,
	i386,
	ARMv9
};

extern Architecture currentArchitecture;
extern System currentSystem;
