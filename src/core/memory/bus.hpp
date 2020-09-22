// This interface is meant to give a consistent manner of laoding and storing
// data in a big endian format. It also gives a good method for mocking RAM
// to test the other components.

#ifndef PB_BUS_HPP
#define PB_BUS_HPP

#include <cstdint>

namespace PurpleBox {
class Bus {
 public:
  virtual ~Bus() = 0;

  virtual uint8_t Read8(uint64_t address) = 0;
  virtual uint16_t Read16(uint64_t address) = 0;
  virtual uint32_t Read32(uint64_t address) = 0;
  virtual uint64_t Read64(uint64_t address) = 0;

  virtual void Write8(uint8_t data, uint64_t address) = 0;
  virtual void Write16(uint16_t data, uint64_t address) = 0;
  virtual void Write32(uint32_t data, uint64_t address) = 0;
  virtual void Write64(uint64_t data, uint64_t address) = 0;
};
}  // namespace PurpleBox

#endif
