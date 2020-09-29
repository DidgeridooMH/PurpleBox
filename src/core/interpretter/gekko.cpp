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
  auto opcode = instruction >> 26 & 0x3F;
  if (m_opcodeFormatTable.find(opcode) == m_opcodeFormatTable.end()) {
    throw std::runtime_error("Unknown opcode " + std::to_string(opcode));
  }
  auto format = m_opcodeFormatTable[opcode](instruction);

  // Execute
  m_opcodeJumpTable[format->GetOpcode()](format);
}

void Gekko::ConnectMemory(std::shared_ptr<Bus> bus) { m_bus = bus; }

void Gekko::GenerateOpcodeTables() {
  CREATE_OPCODE_ENTRY(ADDIS_OPCODE, AddImmShift, DFormat);
  CREATE_OPCODE_ENTRY(ADDI_OPCODE, AddImm, DFormat);
  CREATE_OPCODE_ENTRY(MOVETO_OPCODE, MoveTo, XfxFormat);
  CREATE_OPCODE_ENTRY(STH_OPCODE, StoreHalfword, DFormat);
  CREATE_OPCODE_ENTRY(ORI_OPCODE, OrImm, DFormat);
}

void Gekko::AddImm(std::shared_ptr<Format> format) {
  std::shared_ptr<DFormat> dFormat = std::dynamic_pointer_cast<DFormat>(format);

  uint32_t value = (int16_t)dFormat->GetImmediate();
  if (dFormat->GetA() > 0) {
    value += m_gpr[dFormat->GetA()];
  }
  m_gpr[dFormat->GetD()] = value;

  Debug("$%04x: addi r%d, r%d, 0x%x", m_pc, dFormat->GetD(), dFormat->GetA(),
        dFormat->GetImmediate());
}

void Gekko::AddImmShift(std::shared_ptr<Format> format) {
  auto dFormat = std::dynamic_pointer_cast<DFormat>(format);

  uint32_t value = dFormat->GetImmediate() << 16;
  if (dFormat->GetA() > 0) {
    value += m_gpr[dFormat->GetA()];
  }
  m_gpr[dFormat->GetD()] = value;

  Debug("$%04x: addis r%d, r%d, 0x%x", m_pc, dFormat->GetD(), dFormat->GetA(),
        dFormat->GetImmediate());
}

void Gekko::OrImm(std::shared_ptr<Format> format) {
  auto dFormat = std::dynamic_pointer_cast<DFormat>(format);

  m_gpr[dFormat->GetA()] = m_gpr[dFormat->GetD()] | dFormat->GetImmediate();

  Debug("$%04x: ori r%d, r%d, 0x%x", m_pc, dFormat->GetA(), dFormat->GetD(),
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
