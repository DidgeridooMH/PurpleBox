#include <core/formats/dformat.hpp>

namespace PurpleBox {
DFormat::DFormat(uint32_t instruction)
    : Format(instruction),
      m_D((instruction >> 21) & 0x1F),
      m_A((instruction >> 16) & 0x1F),
      m_imm(static_cast<uint16_t>(instruction & 0xFFFF)) {}

DFormat::~DFormat() {}

uint32_t DFormat::GetD() const { return m_D; }

uint32_t DFormat::GetA() const { return m_A; }

uint16_t DFormat::GetImmediate() const { return m_imm; }
}  // namespace PurpleBox
