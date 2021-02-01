#ifndef PB_OPCODES_HPP
#define PB_OPCODES_HPP

#include <cstdint>

namespace PurpleBox {
static constexpr uint32_t xoFormOeRc(uint32_t base, uint32_t oe, uint32_t rc) {
  return (oe << 9 | (base << 1) | rc);
}

// TODO: Are these correct? Or should they be grouped with other operations?
static constexpr uint32_t ADDIS_OPCODE = 15;
static constexpr uint32_t ADDI_OPCODE = 14;
static constexpr uint32_t STH_OPCODE = 44;
static constexpr uint32_t ORI_OPCODE = 24;

// Extended operations of the XO-Form
static constexpr uint32_t EXTENSION_OPCODE = 31;
static constexpr uint32_t MTSPR_XOPCODE = 467;
static constexpr uint32_t MTMSR_XOPCODE = 146;

static constexpr uint32_t ADD_XOPCODE = xoFormOeRc(266, 0, 0);
static constexpr uint32_t ADDC_XOPCODE = xoFormOeRc(266, 0, 1);
static constexpr uint32_t ADDO_XOPCODE = xoFormOeRc(266, 1, 0);
static constexpr uint32_t ADDOC_XOPCODE = xoFormOeRc(266, 1, 1);
}  // namespace PurpleBox

#endif
