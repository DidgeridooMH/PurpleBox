#ifndef PB_MEMORY_HPP
#define PB_MEMORY_HPP

#include <cstdint>

namespace PurpleBox {
enum class Endian { Little = 0, Big, Native };

uint32_t ReadBigEndianU32(uint32_t* address);
}  // namespace PurpleBox

#endif
