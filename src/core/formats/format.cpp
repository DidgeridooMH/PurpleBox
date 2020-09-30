#include <src/core/formats/format.hpp>

namespace PurpleBox {
Format::Format(uint32_t instruction)
    : m_opcode((instruction >> 26) & 0x3F), m_instruction(instruction) {}

Format::~Format() {}

uint32_t Format::GetOpcode() const { return m_opcode; }

uint32_t Format::GetInstruction() const { return m_instruction; }

}  // namespace PurpleBox
