#include <core/interpretter/gekko.hpp>
#include <utils/log.hpp>

namespace PurpleBox {
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
}  // namespace PurpleBox
