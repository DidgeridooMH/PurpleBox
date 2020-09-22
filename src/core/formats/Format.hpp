#ifndef PB_FORMAT_HPP
#define PB_FORMAT_HPP

#include <cstdint>

namespace PurpleBox {
class Format {
 public:
  Format(uint32_t instruction);
  virtual ~Format() = 0;

  uint32_t GetOpcode() const;

 private:
  uint32_t m_opcode;
};
}  // namespace PurpleBox

#endif
