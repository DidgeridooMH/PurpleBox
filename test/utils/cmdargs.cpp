#include <gtest/gtest.h>

#include <memory>
#include <utils/cmdargs.hpp>

TEST(cmdArgsTest, noArguments) {
  int argc = 1;
  char* argv[] = {"Program", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  args->ParseArguments(argc, argv);
}

TEST(cmdArgsTest, missingArguments) {
  int argc = 1;
  char* argv[] = {"Program", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  args->AddArgument('t', "test", [](const std::string&) { FAIL(); });
  args->ParseArguments(argc, argv);
}

TEST(cmdArgsTest, missingArgumentsWithRequires) {
  int argc = 1;
  char* argv[] = {"Program", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  args->AddArgument(
      't', "test", [](const std::string&) { FAIL(); }, true);
  args->ParseArguments(argc, argv);
}

TEST(cmdArgsTest, hasArgumentShort) {
  int argc = 2;
  char* argv[] = {"Program", "-t", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  auto ran = false;
  auto pRan = &ran;
  args->AddArgument('t', "test", [=](const std::string&) { *pRan = true; });
  args->ParseArguments(argc, argv);
  ASSERT_TRUE(ran);
}

TEST(cmdArgsTest, hasArgumentLong) {
  int argc = 2;
  char* argv[] = {"Program", "--test", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  auto ran = false;
  auto pRan = &ran;
  args->AddArgument('t', "test", [=](const std::string&) { *pRan = true; });
  args->ParseArguments(argc, argv);
  ASSERT_TRUE(ran);
}

TEST(cmdArgsTest, argumentDuplicateShort) {
  int argc = 3;
  char* argv[] = {"Program", "-t", "-t", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  auto ran = 0;
  auto pRan = &ran;
  args->AddArgument('t', "test", [=](const std::string&) { (*pRan)++; });
  args->ParseArguments(argc, argv);
  ASSERT_EQ(ran, 2);
}

TEST(cmdArgsTest, argumentDuplicateLong) {
  int argc = 3;
  char* argv[] = {"Program", "--test", "--test", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  auto ran = 0;
  auto pRan = &ran;
  args->AddArgument('t', "test", [=](const std::string&) { (*pRan)++; });
  args->ParseArguments(argc, argv);
  ASSERT_EQ(ran, 2);
}

TEST(cmdArgsTest, argumentDuplicateMixed) {
  int argc = 3;
  char* argv[] = {"Program", "--test", "-t", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();

  auto ran = 0;
  auto pRan = &ran;
  args->AddArgument('t', "test", [=](const std::string&) { (*pRan)++; });
  args->ParseArguments(argc, argv);
  ASSERT_EQ(ran, 2);
}

TEST(cmdArgsTest, argumentParameter) {
  int argc = 3;
  char* testString = "TEST";
  char* argv[] = {"Program", "-t", testString, NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();
  args->AddArgument(
      't', "test",
      [=](const std::string& parameter) {
        ASSERT_EQ(parameter, std::string(testString));
      },
      true);

  args->ParseArguments(argc, argv);
}

TEST(cmdArgsTest, argumentParameterMixed) {
  int argc = 3;
  char* testString = "TEST";
  char* argv[] = {"Program", "-t", testString, "-x", NULL};

  auto args = std::make_unique<PurpleBox::CmdArgs>();
  args->AddArgument(
      't', "test",
      [=](const std::string& parameter) {
        ASSERT_EQ(parameter, std::string(testString));
      },
      true);

  args->AddArgument(
      'x', "xeno", [=](const std::string&) {}, true);

  args->ParseArguments(argc, argv);
}
