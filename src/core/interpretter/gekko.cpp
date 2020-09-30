#include <core/formats/dformat.hpp>
#include <core/interpretter/gekko.hpp>
#include <core/interpretter/opcodes.hpp>
#include <cstring>
#include <stdexcept>
#include <string>
#include <utils/log.hpp>

namespace PurpleBox {

Gekko::Gekko() {
  GenerateOpcodeTables();
  Reset();
}

void Gekko::Reset() {
  m_gpr.fill(0);
  m_fpr.fill(0);
  m_sr.fill(0);
  m_spr.fill(0);
  m_cr = 0;
  m_fpscr = 0;
  std::memset(&m_msr, 0, sizeof(m_msr));
  m_pc = RESET_VECTOR + 0xfff00000;  // TODO: This is temporary and should be
                                     // handles by the HID registers
}

void Gekko::Tick() {
  // Fetch
  auto instruction = m_bus->Read32(m_pc);
  m_pc += 4;

  // Decode
  auto opcode = DecodeOpcode(instruction);

  // Execute
  m_opcodeJumpTable[opcode](instruction);
}

constexpr uint32_t Gekko::DecodeOpcode(uint32_t instruction) {
  return instruction >> 26 & 0x3F;
}

void Gekko::ConnectMemory(std::shared_ptr<Bus> bus) { m_bus = bus; }

void Gekko::GenerateOpcodeTables() {
  CREATE_OPCODE_ENTRY(ADDIS_OPCODE, AddImmShift, DFormat);
  CREATE_OPCODE_ENTRY(ADDI_OPCODE, AddImm, DFormat);
  CREATE_OPCODE_ENTRY(MOVETO_OPCODE, MoveTo, XfxFormat);
  CREATE_OPCODE_ENTRY(STH_OPCODE, StoreHalfword, DFormat);
  CREATE_OPCODE_ENTRY(ORI_OPCODE, OrImm, DFormat);
}
}  // namespace PurpleBox
