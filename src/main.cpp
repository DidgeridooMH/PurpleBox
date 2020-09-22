#include <core/cdrom.hpp>
#include <core/interpretter/gekko.hpp>
#include <core/memory/ram.hpp>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <utils/cmdargs.hpp>
#include <utils/log.hpp>

#ifndef VERSION
#define VERSION "UNOFFICIAL"
#endif

struct CmdParameters {
  std::string biosFileName;
};

void PrintBanner() {
  std::stringstream banner;
  banner << "PurpleBox Version " << VERSION;
  PurpleBox::Info(banner.str());
}

void PrintUsage() { PurpleBox::Info("Usage: purplebox [ROM]"); }

void HandleCmdArgs(int argc, char** argv,
                   std::shared_ptr<CmdParameters> parameters) {
  auto cmdArgs = std::make_unique<PurpleBox::CmdArgs>();

  cmdArgs->AddArgument('v', "version", [=](const std::string&) {
    PrintBanner();
    exit(0);
  });

  cmdArgs->AddArgument(
      'b', "bios",
      [parameters](const std::string& arg) { parameters->biosFileName = arg; },
      true);

  try {
    cmdArgs->ParseArguments(argc, argv);
  } catch (const std::exception& e) {
    PurpleBox::Error("Error parsing arguments: %s", e.what());
    exit(1);
  }
}

int main(int argc, char** argv) {
  // TODO: Change based on debug/release
  PurpleBox::SetLogLevel(PurpleBox::LogLevel::Debug);

  auto cmdParameters = std::make_shared<CmdParameters>();

  HandleCmdArgs(argc, argv, cmdParameters);

  if (cmdParameters->biosFileName.size() == 0) {
    PurpleBox::Error(
        "HLE Bios not implemented. Please provide an IPL/BIOS file.");
    return 1;
  }

  // auto cdRom = std::make_unique<PurpleBox::CDRom>(romFileName);

  // try {
  //   cdRom->LoadRom();
  // } catch (const std::exception& e) {
  //   PurpleBox::Error("Error reading file " + romFileName + e.what());
  // }

  auto ram = std::make_shared<PurpleBox::Ram>();

  try {
    ram->LoadIplFile(cmdParameters->biosFileName);
  } catch (const std::exception& e) {
    PurpleBox::Error("Error loading BIOS: %s", e.what());
    return 1;
  }

  auto cpu = std::make_unique<PurpleBox::Gekko>();
  cpu->ConnectMemory(ram);

  while (true) {
    try {
      cpu->Tick();
    } catch (const std::exception& e) {
      PurpleBox::Error(e.what());
      return 1;
    }
  }

  return 0;
}
