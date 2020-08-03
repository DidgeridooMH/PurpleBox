#ifndef PB_LOG_HPP
#define PB_LOG_HPP

// TODO: Maybe see if we can make this a stream. This could reduce errors
// involving variadic funcitons.

// TOOD: Make it thread-safe/asynchronous

#include <string>

namespace PurpleBox {

enum class LogLevel { Debug = 0, Info, Warning, Error, None };

void SetLogLevel(LogLevel level);
void Debug(const std::string& format, ...);
void Info(const std::string& format, ...);
void Warn(const std::string& format, ...);
void Error(const std::string& format, ...);

}  // namespace PurpleBox

#endif
