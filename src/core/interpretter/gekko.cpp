#include <core/formats/DFormat.hpp>
#include <core/interpretter/gekko.hpp>
#include <core/interpretter/opcodes.hpp>
#include <cstring>
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
  m_opcodeJumpTable.insert(std::make_pair(
      ADDIS_OPCODE,
      [this](std::shared_ptr<Format> format) { this->AddImmShift(format); }));
  m_opcodeJumpTable.insert(std::make_pair(
      ADDI_OPCODE,
      [this](std::shared_ptr<Format> format) { this->AddImm(format); }));
  m_opcodeJumpTable.insert(std::make_pair(
      MOVETO_OPCODE,
      [this](std::shared_ptr<Format> format) { this->MoveTo(format); }));
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

void Gekko::MoveTo(std::shared_ptr<Format> format) {
  auto xfxFormat = std::dynamic_pointer_cast<XfxFormat>(format);

  switch (xfxFormat->GetExtendedOpcode()) {
    case MTSPR_XOPCODE:
      MoveToSpr(xfxFormat);
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

}  // namespace PurpleBox
