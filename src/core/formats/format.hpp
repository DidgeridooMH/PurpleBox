#ifndef PB_FORMAT_HPP
#define PB_FORMAT_HPP

#include <cstdint>
#include <memory>

namespace PurpleBox {
class Format {
 public:
  Format(uint32_t instruction);
  virtual ~Format() = 0;

  uint32_t GetOpcode() const;
  uint32_t GetInstruction() const;

 private:
  uint32_t m_opcode;
  uint32_t m_instruction;
};
}  // namespace PurpleBox

#endif
