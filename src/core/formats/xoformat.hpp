#ifndef PB_XO_FORMAT_HPP
#define PB_XO_FORMAT_HPP

#include <core/formats/format.hpp>

namespace PurpleBox {
class XoFormat : public Format {
 public:
  XoFormat(uint32_t instruction);
  ~XoFormat() override;

  uint32_t GetRt() const;
  uint32_t GetRa() const;
  uint32_t GetRb() const;
  bool GetOe() const;
  uint32_t GetOx() const;
  bool GetRc() const;

 private:
  uint32_t m_rt;
  uint32_t m_ra;
  uint32_t m_rb;
  bool m_oe;
  uint32_t m_ox;
  bool m_rc;
};
}  // namespace PurpleBox

#endif
