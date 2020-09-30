#ifndef PB_D_FORMAT_HPP
#define PB_D_FORMAT_HPP

#include <core/formats/Format.hpp>

namespace PurpleBox {
class DFormat : public Format {
 public:
  DFormat(uint32_t instruction);
  ~DFormat() override;

  uint32_t GetD() const;
  uint32_t GetA() const;
  uint16_t GetImmediate() const;

 private:
  uint32_t m_D;
  uint32_t m_A;
  uint16_t m_imm;
};
}  // namespace PurpleBox

#endif
