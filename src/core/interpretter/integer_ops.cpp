#include <core/formats/xoformat.hpp>
#include <core/interpretter/gekko.hpp>
#include <utils/log.hpp>

namespace PurpleBox {
void Gekko::UpdateCr0(int32_t result) {
  constexpr uint8_t negative = 1;
  constexpr uint8_t positive = 1 << 1;
  constexpr uint8_t zero = 1 << 2;
  constexpr uint8_t summaryOverflow = 1 << 3;

  uint8_t value = zero;
  if (result < 0) {
    value = negative;
  } else if (result > 0) {
    value = positive;
  }

  m_cr[0] = value;

  if ((m_spr[XER] & 1) > 0) {
    m_cr[0] |= summaryOverflow;
  }
}

void Gekko::UpdateOverflow(int32_t a, int32_t b, int32_t r) {
  constexpr uint32_t overflowBit = 0x2;
  constexpr uint32_t summaryOverflowBit = 0x1;

  // Summary overflow should be set when overflow occurs but not unset when it
  // does not occur.
  m_spr[XER] &= ~0x2;

  if ((a < 0 && b < 0 && r > 0) || (a > 0 && b > 0 && r < 0)) {
    m_spr[XER] |= overflowBit;
    m_spr[XER] |= summaryOverflowBit;
  }
}

void Gekko::AddImm(std::shared_ptr<DFormat> format) {
  uint32_t value = (int16_t)format->GetImmediate();
  if (format->GetA() > 0) {
    value += m_gpr[format->GetA()];
  }
  m_gpr[format->GetD()] = value;

  Debug("${:04X}: addi r{}, r{}, 0x{:X}", m_pc, format->GetD(), format->GetA(),
        format->GetImmediate());
}

void Gekko::AddImmShift(std::shared_ptr<DFormat> format) {
  uint32_t value = format->GetImmediate() << 16;
  if (format->GetA() > 0) {
    value += m_gpr[format->GetA()];
  }
  m_gpr[format->GetD()] = value;

  Debug("${:04X}: addis r{}, r{}, 0x{:X}", m_pc, format->GetD(), format->GetA(),
        format->GetImmediate());
}

void Gekko::Add(std::shared_ptr<XoFormat> format) {
  uint32_t ra = m_gpr[format->GetRa()];
  uint32_t rb = m_gpr[format->GetRb()];

  uint32_t result = ra + rb;
  m_gpr[format->GetRt()] = result;

  if (format->GetOe()) UpdateOverflow(ra, rb, result);
  if (format->GetRc()) UpdateCr0(result);

  Debug("${:04X}: add{}{} r{}, r{}, r{}", format->GetOe() ? "o" : "",
        format->GetRc() ? "." : "", m_pc, format->GetRt(), format->GetRa(),
        format->GetRb());
}

void Gekko::OrImm(std::shared_ptr<DFormat> format) {
  m_gpr[format->GetA()] = m_gpr[format->GetD()] | format->GetImmediate();

  Debug("${:04X}: ori r{}, r{}, 0x{}", m_pc, format->GetA(), format->GetD(),
        format->GetImmediate());
}
}  // namespace PurpleBox