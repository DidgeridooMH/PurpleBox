#ifndef PB_INTERPRETTER_GEKKO_HPP
#define PB_INTERPRETTER_GEKKO_HPP

// TODO: Make constants for the named registers.

#include <array>
#include <core/formats/dformat.hpp>
#include <core/formats/format.hpp>
#include <core/formats/xfxformat.hpp>
#include <core/formats/xoformat.hpp>
#include <core/memory/bus.hpp>
#include <functional>
#include <map>
#include <memory>

namespace PurpleBox {

static constexpr uint32_t RESET_VECTOR = 0x100;

typedef std::function<void(uint32_t)> OpcodeFunc;

#define CREATE_OPCODE_ENTRY(opcode, func, format_t)          \
  m_opcodeJumpTable.insert(                                  \
      std::make_pair(opcode, [this](uint32_t instruction) {  \
        this->func(std::make_shared<format_t>(instruction)); \
      }));
#define CREATE_EX_OPCODE_ENTRY(opcode, exopcode, func, format_t)               \
  m_extendedOpcodeJumpTable.insert(                                            \
      std::make_pair((opcode << 10) | exopcode, [this](uint32_t instruction) { \
        this->func(std::make_shared<format_t>(instruction));                   \
      }));

#define XER 1

class Gekko {
 public:
  Gekko();

  void Tick();

  void ConnectMemory(std::shared_ptr<Bus> bus);

  void Reset();

 private:
  void GenerateOpcodeTables();
  static constexpr uint32_t DecodeOpcode(uint32_t instruction);

  void UpdateCr0(int32_t result);
  void UpdateOverflow(int32_t a, int32_t b, int32_t r);
  void Add(std::shared_ptr<XoFormat> format);
  void AddImm(std::shared_ptr<DFormat> format);
  void AddImmShift(std::shared_ptr<DFormat> format);
  void OrImm(std::shared_ptr<DFormat> format);
  void StoreHalfword(std::shared_ptr<Format> format);
  void ExecuteExtendedOpcode(std::shared_ptr<XfxFormat> format);
  void MoveToSpr(std::shared_ptr<XfxFormat> format);
  void MoveToMsr(std::shared_ptr<XfxFormat> format);

  std::map<uint32_t, OpcodeFunc> m_opcodeJumpTable;
  std::map<uint32_t, OpcodeFunc> m_extendedOpcodeJumpTable;

  std::shared_ptr<Bus> m_bus;
  std::array<uint32_t, 32> m_gpr;
  std::array<float, 32> m_fpr;
  std::array<uint32_t, 16> m_sr;  // Not used in DolphinOS
  std::array<uint32_t, 1024> m_spr;

  std::array<uint8_t, 8> m_cr;
  uint32_t m_fpscr;
  union {
    struct msr {
      uint32_t le : 1;
      uint32_t ri : 1;
      uint32_t pm : 1;
      uint32_t reserved0 : 1;
      uint32_t dr : 1;
      uint32_t ir : 1;
      uint32_t ip : 1;
      uint32_t reserved1 : 1;
      uint32_t fe1 : 1;
      uint32_t be : 1;
      uint32_t se : 1;
      uint32_t fe0 : 1;
      uint32_t me : 1;
      uint32_t fp : 1;
      uint32_t pr : 1;
      uint32_t ee : 1;
      uint32_t ile : 1;
      uint32_t reserved2 : 1;
      uint32_t pow : 1;
      uint32_t reserved3;
    };
    uint32_t raw;
  } m_msr;
  uint32_t m_pc;
};
}  // namespace PurpleBox

#endif
