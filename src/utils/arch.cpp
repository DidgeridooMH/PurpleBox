#include <iostream>
#include <utils/arch.hpp>

namespace PurpleBox {

static Endian GetNativeEndianness() {
  static const int testWord = 1;
  return (char*)(&testWord)[0] ? Endian::Little : Endian::Big;
}

uint32_t ReadBigEndianU32(void* address) {
  if (GetNativeEndianness() == Endian::Big) {
    return *(uint32_t*)address;
  }

  uint32_t out = 0;
  for (auto i = 0; i < sizeof(uint32_t); ++i) {
    out |= ((uint32_t)((uint8_t*)address)[i])
           << ((sizeof(uint32_t) - 1 - i) * 8);
  }

  return out;
}

}  // namespace PurpleBox
