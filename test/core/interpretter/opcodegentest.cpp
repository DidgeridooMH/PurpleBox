#include <gtest/gtest.h>

#include <core/interpretter/opcodes.hpp>

TEST(opcodeGenTest, xoFormNoOeNoRc) {
  uint32_t opcode = 230;

  uint32_t generatedCode = PurpleBox::xoFormOeRc(opcode, 0, 0);

  ASSERT_EQ(generatedCode, 230 << 1);
}

TEST(opcodeGenTest, xoFormOeNoRc) {
  uint32_t opcode = 230;

  uint32_t generatedCode = PurpleBox::xoFormOeRc(opcode, 1, 0);

  ASSERT_EQ(generatedCode, 512 + (230 << 1));
}

TEST(opcodeGenTest, xoFormNoOeRc) {
  uint32_t opcode = 230;

  uint32_t generatedCode = PurpleBox::xoFormOeRc(opcode, 0, 1);

  ASSERT_EQ(generatedCode, (230 << 1) | 1);
}

TEST(opcodeGenTest, xoFormOeRc) {
  uint32_t opcode = 230;

  uint32_t generatedCode = PurpleBox::xoFormOeRc(opcode, 1, 1);

  ASSERT_EQ(generatedCode, 512 + ((230 << 1) | 1));
}
