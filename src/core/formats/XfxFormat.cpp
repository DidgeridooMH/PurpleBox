#include <core/formats/XfxFormat.hpp>

namespace PurpleBox {
XfxFormat::XfxFormat(uint32_t instruction)
    : Format(instruction),
      m_extendedOp(instruction >> 1 & 0x3FF),
      m_A(instruction >> 21 & 0x1F),
      m_B(instruction >> 11 & 0x3FF) {}

XfxFormat::~XfxFormat() {}

uint32_t XfxFormat::GetA() const { return m_A; }

uint32_t XfxFormat::GetB() const { return m_B; }

uint32_t XfxFormat::GetExtendedOpcode() const { return m_extendedOp; }
}  // namespace PurpleBox
