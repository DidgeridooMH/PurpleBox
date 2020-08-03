#include <core/cdrom.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utils/log.hpp>

#ifndef VERSION
#define VERSION "UNOFFICIAL"
#endif

using namespace PurpleBox;

void PrintBanner() {
  std::stringstream banner;
  banner << "PurpleBox Version " << VERSION;
  Info(banner.str());
}

void PrintUsage() { Info("Usage: purplebox [ROM]"); }

int main(int argc, char** argv) {
  // TODO: Change based on debug/release
  SetLogLevel(LogLevel::Debug);

  PrintBanner();
  if (argc < 2) {
    Error("File not specified");
    PrintUsage();

    return 1;
  }

  std::string romFileName = argv[1];
  auto cdRom = std::make_unique<CDRom>(romFileName);

  try {
    cdRom->LoadRom();
  } catch (const std::exception& e) {
    Error("Error reading file " + romFileName + e.what());
  }

  return 0;
}
