#ifndef PB_OPCODES_HPP
#define PB_OPCODES_HPP

#include <cstdint>

namespace PurpleBox {
static constexpr uint32_t ADDIS_OPCODE = 15;
static constexpr uint32_t ADDI_OPCODE = 14;
static constexpr uint32_t MOVETO_OPCODE = 31;
static constexpr uint32_t STH_OPCODE = 44;
static constexpr uint32_t ORI_OPCODE = 24;

static constexpr uint32_t MTSPR_XOPCODE = 467;
static constexpr uint32_t MTMSR_XOPCODE = 146;
}  // namespace PurpleBox

#endif
