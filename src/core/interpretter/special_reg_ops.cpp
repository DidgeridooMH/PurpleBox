#include <core/interpretter/gekko.hpp>
#include <core/interpretter/opcodes.hpp>
#include <stdexcept>
#include <utils/log.hpp>

namespace PurpleBox {
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
