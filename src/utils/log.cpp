#include <cstdarg>
#include <cstdio>
#include <utils/log.hpp>

#define PrintLevel(level, format) \
  va_list argp;                   \
  va_start(argp, format);         \
  Print(level, format, argp);     \
  va_end(argp);

namespace PurpleBox {

static LogLevel g_level = LogLevel::Warning;

void SetLogLevel(LogLevel level) { g_level = level; }

static void Print(LogLevel level, const std::string& format, va_list args) {
  if (level >= g_level) {
    auto outStream = level >= LogLevel::Warning ? stderr : stdout;
    vfprintf(outStream, format.c_str(), args);
    fprintf(outStream, "\n");
    fflush(outStream);
  }
}

void Debug(const std::string& format, ...) {
  PrintLevel(LogLevel::Debug, format);
}

void Info(const std::string& format, ...) {
  PrintLevel(LogLevel::Info, format);
}

void Warn(const std::string& format, ...) {
  PrintLevel(LogLevel::Warning, format);
}

void Error(const std::string& format, ...) {
  PrintLevel(LogLevel::Error, format);
}
}  // namespace PurpleBox