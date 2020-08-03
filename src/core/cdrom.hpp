#ifndef PB_CD_ROM_HPP
#define PB_CD_ROM_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>

namespace PurpleBox {
class CDRom {
 public:
  CDRom(const std::string& romName);

  void LoadRom();

 private:
  std::string m_romName;

  std::vector<uint8_t> m_bootFile;
  std::vector<uint8_t> m_appLoaderFile;
  std::vector<uint8_t> m_fstFile;
};
}  // namespace PurpleBox

#endif
