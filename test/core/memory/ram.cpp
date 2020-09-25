#include <gtest/gtest.h>

#include <core/memory/ram.hpp>
#include <memory>

TEST(ramTest, singleReadWrite) {
  constexpr uint8_t testValue = 0xDEU;
  constexpr uint64_t address = 0x0U;
  auto ram = std::make_unique<PurpleBox::Ram>();

  ram->Write8(testValue, address);
  auto result = ram->Read8(address);

  ASSERT_EQ(testValue, result);
}

TEST(ramTest, highSingleReadWrite) {
  constexpr uint8_t testValue = 0xDEU;
  constexpr uint64_t address = 0xFFFFFFFFFFFEU;
  auto ram = std::make_unique<PurpleBox::Ram>();

  ram->Write8(testValue, address);
  auto result = ram->Read8(address);

  ASSERT_EQ(testValue, result);
}

TEST(ramTest, halfWordReadWrite) {
  constexpr uint16_t testValue = 0xDEADU;
  constexpr uint64_t address = 0x0U;
  auto ram = std::make_unique<PurpleBox::Ram>();

  ram->Write16(testValue, address);
  auto result = ram->Read16(address);

  ASSERT_EQ(testValue, result);
}

TEST(ramTest, wordReadWrite) {
  constexpr uint32_t testValue = 0xDEADBEEFU;
  constexpr uint64_t address = 0x0U;
  auto ram = std::make_unique<PurpleBox::Ram>();

  ram->Write32(testValue, address);
  auto result = ram->Read32(address);

  ASSERT_EQ(testValue, result);
}

TEST(ramTest, doubleWordReadWrite) {
  constexpr uint64_t testValue = 0xDEADBEEF12345678U;
  constexpr uint64_t address = 0x0U;
  auto ram = std::make_unique<PurpleBox::Ram>();

  ram->Write64(testValue, address);
  auto result = ram->Read64(address);

  ASSERT_EQ(testValue, result);
}

TEST(ramTest, sequentialReadWrite) {
  constexpr uint8_t testValues[] = {0xDE, 0xAD, 0xBE, 0xEF};
  constexpr uint64_t addressSpaceSize = PAGE_SIZE * 2;
  auto ram = std::make_unique<PurpleBox::Ram>();

  for (uint64_t i = 0; i < addressSpaceSize; i++) {
    ram->Write8(testValues[i % sizeof(testValues)], i);
  }

  for (uint64_t i = 0; i < addressSpaceSize; i++) {
    auto result = ram->Read8(i);
    ASSERT_EQ(result, testValues[i % sizeof(testValues)]);
  }
}
