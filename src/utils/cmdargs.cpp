#include <queue>
#include <stdexcept>
#include <utils/cmdargs.hpp>
#include <utils/log.hpp>

namespace PurpleBox {
void CmdArgs::AddArgument(char flag, const std::string& field,
                          std::function<void(const std::string&)> callback,
                          bool hasParam) {
  if (field.size() == 0) {
    throw std::runtime_error("Invalid field argument");
  }

  if (flag != '\0') {
    m_flagToFieldMap.insert(std::make_pair(flag, field));
  }

  m_fieldCallbackMap.insert(
      std::make_pair(std::string(field), ArgField({callback, hasParam})));
}

void CmdArgs::ParseArguments(int argc, char** argv) {
  auto argsQueue = std::make_unique<std::queue<std::string>>();

  for (int i = 1; i < argc; ++i) argsQueue->push(std::string(argv[i]));

  while (argsQueue->size() > 0) {
    auto flag = argsQueue->front();
    argsQueue->pop();

    std::string fieldName = "";
    if (flag.at(0) == '-' && flag.size() > 1) {
      if (flag.at(1) == '-') {
        fieldName = flag.substr(2);
      } else if (m_flagToFieldMap.count(flag.at(1)) > 0) {
        fieldName = m_flagToFieldMap[flag.at(1)];
      } else {
        throw std::runtime_error("Invalid argument " + flag);
      }
    } else {
      throw std::runtime_error("Invalid argument " + flag);
    }

    if (m_fieldCallbackMap.count(fieldName) == 0)
      throw std::runtime_error("Invalid argument " + flag + " - " + fieldName);

    std::string parameter = "";
    if (m_fieldCallbackMap[fieldName].parameterRequired) {
      if (argsQueue->size() == 0)
        throw std::runtime_error("Missing argument for " + flag);

      parameter = argsQueue->front();
      argsQueue->pop();
    }

    m_fieldCallbackMap[fieldName].callback(parameter);
  }
}
}  // namespace PurpleBox
