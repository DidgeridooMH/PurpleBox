#include <core/memory/ram.hpp>
#include <exception>
#include <fstream>
#include <utils/log.hpp>

namespace PurpleBox {
Ram::Ram() : m_pageTable(std::make_unique<PageTable>()) {}

Ram::~Ram() {}

uint8_t Ram::Read8(uint64_t address) {
  const auto hash = address / PAGE_SIZE;

  if (m_pageTable->count(hash) == 0) {
    return 0;
  }

  return (*(*m_pageTable)[hash])[address % PAGE_SIZE];
}

uint16_t Ram::Read16(uint64_t address) {
  return (static_cast<uint16_t>(Read8(address)) << 8) |
         static_cast<uint16_t>(Read8(address + 1));
}

uint32_t Ram::Read32(uint64_t address) {
  return (static_cast<uint32_t>(Read16(address)) << 16) |
         static_cast<uint32_t>(Read16(address + 2));
}

uint64_t Ram::Read64(uint64_t address) {
  return (static_cast<uint64_t>(Read32(address)) << 32) |
         static_cast<uint64_t>(Read32(address + 4));
}

void Ram::Write8(uint8_t data, uint64_t address) {
  const auto hash = address / PAGE_SIZE;

  if (m_pageTable->count(hash) == 0) {
    m_pageTable->insert(std::make_pair(hash, std::make_shared<Page>()));
  }

  (*(*m_pageTable)[hash])[address % PAGE_SIZE] = data;
}
void Ram::Write16(uint16_t data, uint64_t address) {
  Write8((data >> 8) & 0xFF, address);
  Write8(data & 0xFF, address + 1);
}
void Ram::Write32(uint32_t data, uint64_t address) {
  Write16((data >> 16) & 0xFFFF, address);
  Write16(data & 0xFFFF, address + 2);
}
void Ram::Write64(uint64_t data, uint64_t address) {
  Write32((data >> 32) & 0xFFFFFFFF, address);
  Write32(data & 0xFFFFFFFF, address + 4);
}

void Ram::LoadIplFile(const std::string& filename) {
  PurpleBox::Info("Loading BIOS File: %s", filename.c_str());
  auto biosFile = std::ifstream(filename, std::ios::ate | std::ios::binary);
  if (!biosFile.is_open()) {
    throw std::runtime_error("Unable to open file");
  }

  auto biosFileSize = biosFile.tellg();
  PurpleBox::Debug("BIOS file size: %ld", biosFileSize);
  biosFile.seekg(biosFile.beg);

  try {
    static const uint64_t biosMemoryPage = 0xfff00000;
    auto bios = std::make_unique<uint8_t[]>(biosFileSize);
    biosFile.read(reinterpret_cast<char*>(bios.get()), biosFileSize);
    for (int i = 0; i < biosFileSize; ++i) {
      Write8(bios[i], biosMemoryPage + i);
    }
  } catch (const std::exception& e) {
    throw std::runtime_error("Unable to read file");
  }
}

}  // namespace PurpleBox
