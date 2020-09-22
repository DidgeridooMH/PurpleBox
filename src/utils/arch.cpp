#include <utils/arch.hpp>

namespace PurpleBox {

static Endian GetNativeEndianness() {
  static const int testWord = 1;
  return (char*)(&testWord)[0] ? Endian::Little : Endian::Big;
}

uint32_t ReadBigEndianU32(uint32_t* address) {
  if (GetNativeEndianness() == Endian::Big) {
    return *address;
  }

  auto out = 0;
  for (uint32_t i = 0; i < sizeof(uint32_t); ++i) {
    out |= (static_cast<uint32_t>(address[i]) & 0xFF)
           << (8 * (sizeof(uint32_t) - i - 1));
  }
  return out;
}

}  // namespace PurpleBox
