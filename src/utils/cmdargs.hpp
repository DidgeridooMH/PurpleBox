#ifndef PB_CMDARGS_HPP
#define PB_CMDARGS_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>

namespace PurpleBox {

typedef struct ArgField {
  std::function<void(const std::string&)> callback;
  bool parameterRequired;
} ArgField;

typedef std::map<std::string, std::string> ArgsMap;
typedef std::map<std::string, ArgField> FieldCallbackMap;

// TODO: Provide a usage print function.
class CmdArgs {
 public:
  void AddArgument(char flag, const std::string& field,
                   std::function<void(const std::string&)> callback,
                   bool hasParam = false);
  void ParseArguments(int argc, char** argv);

 private:
  std::map<char, std::string> m_flagToFieldMap;
  FieldCallbackMap m_fieldCallbackMap;
};
}  // namespace PurpleBox

#endif
