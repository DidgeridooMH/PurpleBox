#ifndef PB_RAM_HPP
#define PB_RAM_HPP

#include <array>
#include <core/memory/bus.hpp>
#include <map>
#include <memory>

#define PAGE_SIZE 0x10000UL

namespace PurpleBox {

typedef std::array<uint8_t, PAGE_SIZE> Page;
typedef std::map<uint64_t, std::shared_ptr<Page>> PageTable;

class Ram : public Bus {
 public:
  Ram();
  ~Ram() override;

  uint8_t Read8(uint64_t address) override;
  uint16_t Read16(uint64_t address) override;
  uint32_t Read32(uint64_t address) override;
  uint64_t Read64(uint64_t address) override;

  void Write8(uint8_t data, uint64_t address) override;
  void Write16(uint16_t data, uint64_t address) override;
  void Write32(uint32_t data, uint64_t address) override;
  void Write64(uint64_t data, uint64_t address) override;

  void LoadIplFile(const std::string& filename);

 private:
  std::unique_ptr<PageTable> m_pageTable;
};
}  // namespace PurpleBox

#endif
