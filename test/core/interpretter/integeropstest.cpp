#include <gtest/gtest.h>

#define protected public
#define private public
#include <core/interpretter/gekko.hpp>

TEST(integerOps, updateOverflowPosPosPos) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateOverflow(1, 1, 1);

  ASSERT_EQ(cpu->m_spr[XER], 0);
}

TEST(integerOps, updateOverflowNegPosPos) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateOverflow(-1, 3, 1);

  ASSERT_EQ(cpu->m_spr[XER], 0);
}

TEST(integerOps, updateOverflowPosPosNeg) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateOverflow(1, 1, -1);

  ASSERT_EQ(cpu->m_spr[XER], 0b11);
}

TEST(integerOps, updateOverflowNegNegPos) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateOverflow(-1, -1, 1);

  ASSERT_EQ(cpu->m_spr[XER], 0b11);
}

TEST(integerOps, updateCr0Zero) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateCr0(0);

  ASSERT_EQ(cpu->m_cr[0], 0b0100);
}

TEST(integerOps, updateCr0Negative) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateCr0(-1);

  ASSERT_EQ(cpu->m_cr[0], 0b0001);
}

TEST(integerOps, updateCr0Positive) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateCr0(4);

  ASSERT_EQ(cpu->m_cr[0], 0b0010);
}

TEST(integerOps, updateCr0SO) {
  auto cpu = std::make_unique<PurpleBox::Gekko>();

  cpu->UpdateOverflow(1, 2, -1);
  cpu->UpdateCr0(-1);

  ASSERT_EQ(cpu->m_cr[0], 0b1001);
}

static inline std::shared_ptr<PurpleBox::DFormat> buildDFormat(
    uint32_t argD, uint32_t argA, uint32_t immediate) {
  uint32_t instruction = (argD << 21U) | (argA << 16U) | (immediate & 0xFFFF);
  return std::make_unique<PurpleBox::DFormat>(instruction);
}

TEST(gekkoAddImmTest, addImmNonZeroSrc) {
  constexpr uint32_t dest = 3U;
  constexpr uint32_t src = 1U;
  constexpr int16_t testImmediate = 0xDEAD;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], testImmediate + testResident);
}

TEST(gekkoAddImmTest, addImmZeroSrc) {
  constexpr auto dest = 3U;
  constexpr auto src = 0U;
  constexpr uint16_t testImmediate = 0xDEADU;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], (int16_t)testImmediate);
}

TEST(gekkoAddImmTest, addImmZeroSameSrc) {
  constexpr auto dest = 3U;
  constexpr auto src = 3U;
  constexpr uint16_t testImmediate = 0xDEADU;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], (int16_t)testImmediate + testResident);
}

TEST(gekkoAddImmShiftTest, addImmShiftNonZeroSrc) {
  constexpr uint32_t dest = 3U;
  constexpr uint32_t src = 1U;
  constexpr int16_t testImmediate = 0xDEAD;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImmShift(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], ((uint32_t)testImmediate << 16) + testResident);
}

TEST(gekkoAddImmShiftTest, addImmShiftZeroSrc) {
  constexpr auto dest = 3U;
  constexpr auto src = 0U;
  constexpr int16_t testImmediate = 0xDEADU;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImmShift(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], ((uint32_t)testImmediate << 16));
}

TEST(gekkoAddImmShiftTest, addImmShiftZeroSameSrc) {
  constexpr auto dest = 3U;
  constexpr auto src = 3U;
  constexpr int16_t testImmediate = 0xDEADU;
  constexpr uint32_t testResident = 0xFAADEADA;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(dest, src, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->AddImmShift(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], ((uint32_t)testImmediate << 16) + testResident);
}

TEST(gekkoOrImm, orImmZeroZero) {
  constexpr auto dest = 3U;
  constexpr auto src = 2U;
  constexpr int16_t testImmediate = 0;
  constexpr uint32_t testResident = 0;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(src, dest, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->OrImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], 0);
}

TEST(gekkoOrImm, orImmZeroNonZero) {
  constexpr auto dest = 3U;
  constexpr auto src = 2U;
  constexpr int16_t testImmediate = 0x1FED;
  constexpr uint32_t testResident = 0;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(src, dest, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->OrImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], 0x1FED);
}

TEST(gekkoOrImm, orImmNonZeroNonZero) {
  constexpr auto dest = 3U;
  constexpr auto src = 2U;
  constexpr int16_t testImmediate = 0x1FED;
  constexpr uint32_t testResident = 0xF000F00D;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(src, dest, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->OrImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], testResident | testImmediate);
}

TEST(gekkoOrImm, orImmSameReg) {
  constexpr auto dest = 3U;
  constexpr auto src = 3U;
  constexpr int16_t testImmediate = 0x1FED;
  constexpr uint32_t testResident = 0xF000F00D;
  auto cpu = std::make_unique<PurpleBox::Gekko>();
  auto instruction = buildDFormat(src, dest, testImmediate);
  cpu->m_gpr[src] = testResident;

  cpu->OrImm(instruction);

  ASSERT_EQ(cpu->m_gpr[dest], testResident | testImmediate);
}
