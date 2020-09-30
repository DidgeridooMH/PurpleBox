#include <core/interpretter/gekko.hpp>
#include <core/interpretter/opcodes.hpp>
#include <stdexcept>
#include <utils/log.hpp>

namespace PurpleBox {
void Gekko::MoveToSpr(std::shared_ptr<XfxFormat> format) {
  m_spr[format->GetB()] = m_gpr[format->GetA()];

  Debug("$%04x: mtspr spr%d, r%d", m_pc, format->GetB(), format->GetA());
}

void Gekko::MoveToMsr(std::shared_ptr<XfxFormat> format) {
  m_msr.raw = m_gpr[format->GetA()];

  Debug("$%04x: mtmsr r%d", m_pc, format->GetA());
}
}  // namespace PurpleBox
