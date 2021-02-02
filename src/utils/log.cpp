#include <cstdarg>
#include <cstdio>

#include <fmt/printf.h>

#include <utils/log.hpp>

namespace PurpleBox {

static LogLevel g_level = LogLevel::Warning;

void SetLogLevel(LogLevel level) { g_level = level; }

void Print(LogLevel level, const fmt::string_view format, fmt::format_args args) {
  if (level >= g_level) {
    auto outStream = level >= LogLevel::Warning ? stderr : stdout;
    fmt::vprint(outStream, format, args);
    fmt::print(outStream, "\n");
  }
}
}  // namespace PurpleBox