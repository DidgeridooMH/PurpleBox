#include <core/formats/xoformat.hpp>

namespace PurpleBox {
XoFormat::XoFormat(uint32_t instruction)
    : Format(instruction),
      m_rt(instruction >> 21 & 0x1F),
      m_ra(instruction >> 16 & 0x1F),
      m_rb(instruction >> 11 & 0x1F),
      m_oe((instruction & 0x400) > 0),
      m_ox(instruction >> 1 & 0x1FF),
      m_rc((instruction & 0x1) > 0) {}

XoFormat::~XoFormat() {}

uint32_t XoFormat::GetRt() const { return m_rt; }

uint32_t XoFormat::GetRa() const { return m_ra; }

uint32_t XoFormat::GetRb() const { return m_rb; }

bool XoFormat::GetOe() const { return m_oe; }

uint32_t XoFormat::GetOx() const { return m_ox; }

bool XoFormat::GetRc() const { return m_rc; }
}  // namespace PurpleBox
