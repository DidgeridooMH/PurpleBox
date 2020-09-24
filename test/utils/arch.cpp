#include <gtest/gtest.h>

#include <utils/arch.hpp>

TEST(archTest, ReadBigEndianU32Zero) {
  uint32_t num = 0;

  std::stringstream value;
  value << std::hex << PurpleBox::ReadBigEndianU32(&num);

  EXPECT_EQ(value.str(), "0");
}

TEST(archTest, ReadBigEndianU32Num) {
  uint32_t num = 0xEFBEADDE;

  std::stringstream value;
  value << std::hex << PurpleBox::ReadBigEndianU32(&num);

  EXPECT_EQ(value.str(), "deadbeef");
}

TEST(archTest, ReadBigEndianU32Common) {
  uint8_t num[] = {0xDE, 0xAD, 0xBE, 0xEF};

  std::stringstream value;
  value << std::hex << PurpleBox::ReadBigEndianU32(num);

  EXPECT_EQ(value.str(), "deadbeef");
}
