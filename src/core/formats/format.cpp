#include <src/core/formats/Format.hpp>

namespace PurpleBox {
Format::Format(uint32_t instruction) : m_opcode((instruction >> 26) & 0x3F) {}

Format::~Format() {}

uint32_t Format::GetOpcode() const { return m_opcode; }

}  // namespace PurpleBox
