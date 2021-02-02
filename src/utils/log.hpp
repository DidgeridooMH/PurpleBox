#ifndef PB_LOG_HPP
#define PB_LOG_HPP

#include <fmt/format.h>

// TOOD: Make it thread-safe/asynchronous

#include <string>

namespace PurpleBox {

enum class LogLevel { Debug = 0, Info, Warning, Error, None };

void SetLogLevel(LogLevel level);

void Print(LogLevel level, const fmt::string_view format, fmt::format_args args);

template <typename S, typename... Args>
void Debug(const S& format, Args&&... args) {
  Print(LogLevel::Debug, format, fmt::make_args_checked<Args...>(format, args...));
}

template <typename S, typename... Args>
void Info(const S& format, Args&&... args) {
  Print(LogLevel::Info, format, fmt::make_args_checked<Args...>(format, args...));
}
template <typename S, typename... Args>
void Warning(const S& format, Args&&... args) {
  Print(LogLevel::Warning, format, fmt::make_args_checked<Args...>(format, args...));
}

template <typename S, typename... Args>
void Error(const S& format, Args&&... args) {
  Print(LogLevel::Error, format, fmt::make_args_checked<Args...>(format, args...));
}
}  // namespace PurpleBox

#endif
