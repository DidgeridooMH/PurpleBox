#ifndef PB_XFX_FORMAT_HPP
#define PB_XFX_FORMAT_HPP

#include <core/formats/format.hpp>

namespace PurpleBox {
class XfxFormat : public Format {
 public:
  XfxFormat(uint32_t instruction);
  ~XfxFormat() override;

  uint32_t GetExtendedOpcode() const;
  uint32_t GetA() const;
  uint32_t GetB() const;

 private:
  uint32_t m_extendedOp;
  uint32_t m_A;
  uint32_t m_B;
};
}  // namespace PurpleBox

#endif
