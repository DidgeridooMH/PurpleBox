#include <core/cdrom.hpp>
#include <fstream>
#include <utils/arch.hpp>
#include <utils/log.hpp>

namespace PurpleBox {

CDRom::CDRom(const std::string& romName) : m_romName(romName) {}

void CDRom::LoadRom() {
  static const auto BootBinSize = 0x440;

  std::ifstream romFile(m_romName, std::ios::binary);

  Info("Loading boot.bin...");
  m_bootFile = std::vector<uint8_t>(BootBinSize);
  romFile.read(reinterpret_cast<char*>(m_bootFile.data()), BootBinSize);

  static const auto Bi2BinSize = 0x2000;
  romFile.seekg(BootBinSize + Bi2BinSize);

  Info("Loading appldr.bin...");
  static const auto AppLoaderHeaderSize = 0x20;
  m_appLoaderFile = std::vector<uint8_t>(AppLoaderHeaderSize);
  romFile.read(reinterpret_cast<char*>(m_appLoaderFile.data()),
               AppLoaderHeaderSize);

  static const auto AppLoaderSizeOffset = 0x14;
  auto appLoaderSize = ReadBigEndianU32(
      reinterpret_cast<uint32_t*>(&m_appLoaderFile[AppLoaderSizeOffset]));

  m_appLoaderFile.reserve(appLoaderSize);
  romFile.read(
      reinterpret_cast<char*>(&m_appLoaderFile.data()[AppLoaderHeaderSize]),
      appLoaderSize);
}

}  // namespace PurpleBox
