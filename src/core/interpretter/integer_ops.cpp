#include <core/interpretter/gekko.hpp>
#include <utils/log.hpp>

namespace PurpleBox {
void Gekko::AddImm(std::shared_ptr<DFormat> format) {
  uint32_t value = (int16_t)format->GetImmediate();
  if (format->GetA() > 0) {
    value += m_gpr[format->GetA()];
  }
  m_gpr[format->GetD()] = value;

  Debug("$%04x: addi r%d, r%d, 0x%x", m_pc, format->GetD(), format->GetA(),
        format->GetImmediate());
}

void Gekko::AddImmShift(std::shared_ptr<DFormat> format) {
  uint32_t value = format->GetImmediate() << 16;
  if (format->GetA() > 0) {
    value += m_gpr[format->GetA()];
  }
  m_gpr[format->GetD()] = value;

  Debug("$%04x: addis r%d, r%d, 0x%x", m_pc, format->GetD(), format->GetA(),
        format->GetImmediate());
}

void Gekko::OrImm(std::shared_ptr<DFormat> format) {
  m_gpr[format->GetA()] = m_gpr[format->GetD()] | format->GetImmediate();

  Debug("$%04x: ori r%d, r%d, 0x%x", m_pc, format->GetA(), format->GetD(),
        format->GetImmediate());
}
}  // namespace PurpleBox