#include <core/formats/DFormat.hpp>
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
  auto format = DecodeInstruction(instruction);

  // Execute
  m_opcodeJumpTable[format->GetOpcode()](format);
}

void Gekko::ConnectMemory(std::shared_ptr<Bus> bus) { m_bus = bus; }

void Gekko::GenerateOpcodeTables() {
  CREATE_OPCODE_ENTRY(ADDIS_OPCODE, AddImmShift);
  CREATE_OPCODE_ENTRY(ADDI_OPCODE, AddImm);
  CREATE_OPCODE_ENTRY(MOVETO_OPCODE, MoveTo);
  CREATE_OPCODE_ENTRY(STH_OPCODE, StoreHalfword);
}

std::shared_ptr<Format> Gekko::DecodeInstruction(uint32_t instruction) {
  auto opcode = instruction >> 26 & 0x3F;

  // TODO: EFFICIENCYYYY
  switch (opcode) {
    case MOVETO_OPCODE:
      // TODO: This most likely will change.
      return std::make_shared<XfxFormat>(instruction);
    case ADDI_OPCODE:
    case ADDIS_OPCODE:
    case STH_OPCODE:
      return std::make_shared<DFormat>(instruction);
  }

  throw std::runtime_error("Unknown opcode " + std::to_string(opcode));
}

void Gekko::AddImm(std::shared_ptr<Format> format) {
  std::shared_ptr<DFormat> dFormat = std::dynamic_pointer_cast<DFormat>(format);

  m_gpr[dFormat->GetD()] = dFormat->GetImmediate();
  if (dFormat->GetA() > 0) {
    m_gpr[dFormat->GetD()] = m_gpr[dFormat->GetA()];
  }

  Debug("$%04x: addi\tr%d, r%d, 0x%x", m_pc, dFormat->GetD(), dFormat->GetA(),
        dFormat->GetImmediate());
}

void Gekko::AddImmShift(std::shared_ptr<Format> format) {
  auto dFormat = std::dynamic_pointer_cast<DFormat>(format);

  m_gpr[dFormat->GetD()] = dFormat->GetImmediate() << 16;
  if (dFormat->GetA() > 0) {
    m_gpr[dFormat->GetD()] = m_gpr[dFormat->GetA()];
  }

  Debug("$%04x: addis r%d, r%d, 0x%x", m_pc, dFormat->GetD(), dFormat->GetA(),
        dFormat->GetImmediate());
}

void Gekko::StoreHalfword(std::shared_ptr<Format> format) {
  auto dFormat = std::dynamic_pointer_cast<DFormat>(format);

  int16_t signedImm = static_cast<int16_t>(dFormat->GetImmediate());
  uint32_t address = static_cast<int32_t>(signedImm);
  if (dFormat->GetA() > 0) {
    address += m_gpr[dFormat->GetA()];
  }

  uint16_t value =
      (m_gpr[dFormat->GetD()] & 0xFF) | (m_gpr[dFormat->GetD()] >> 8 & 0xFF);

  m_bus->Write16(value, address);

  Debug("$%04x: sth r%d, %d(r%d)", m_pc, dFormat->GetD(),
        dFormat->GetImmediate(), dFormat->GetA());
}

void Gekko::MoveTo(std::shared_ptr<Format> format) {
  auto xfxFormat = std::dynamic_pointer_cast<XfxFormat>(format);

  switch (xfxFormat->GetExtendedOpcode()) {
    case MTSPR_XOPCODE:
      MoveToSpr(xfxFormat);
      break;
    case MTMSR_XOPCODE:
      MoveToMsr(xfxFormat);
      break;
    default:
      throw std::runtime_error("Unknown extended move to opcode " +
                               std::to_string(xfxFormat->GetExtendedOpcode()));
  }
}

void Gekko::MoveToSpr(std::shared_ptr<XfxFormat> format) {
  m_spr[format->GetB()] = m_gpr[format->GetA()];

  Debug("$%04x: mtspr spr%d, r%d", m_pc, format->GetB(), format->GetA());
}

void Gekko::MoveToMsr(std::shared_ptr<XfxFormat> format) {
  m_msr.raw = m_gpr[format->GetA()];

  Debug("$%04x: mtmsr r%d", m_pc, format->GetA());
}

}  // namespace PurpleBox
