#pragma once

#include "Argument.h"

namespace ArgumentParser {

class ArgParser {
public:
	ArgParser(const char* name);
    ~ArgParser();
    Argument<bool>& AddFlag(const char* long_name);
    Argument<bool>& AddFlag(const char short_name, const char* long_name);
    Argument<bool>& AddFlag(const char* long_name, const char* description);
    Argument<bool>& AddFlag(const char short_name, const char* long_name, const char* description);
    Argument<int>& AddIntArgument(const char* long_name);
    Argument<int>& AddIntArgument(const char short_name, const char* long_name);
    Argument<int>& AddIntArgument(const char* long_name, const char* description);
    Argument<int>& AddIntArgument(const char short_name, const char* long_name, const char* description);
    Argument<std::string>& AddStringArgument(const char* long_name);
    Argument<std::string>& AddStringArgument(const char short_name, const char* long_name);
    Argument<std::string>& AddStringArgument(const char* long_name, const char* description);
    Argument<std::string>& AddStringArgument(const char short_name, const char* long_name, const char* description);
    bool Parse(const char** argv, int argc);
    bool GetFlag(const char* name_argument);
    size_t GetIntValue(const char* name_argument);
    size_t GetIntValue(const char* name_argument, size_t index);
    std::string GetStringValue(const char* name_argument);
	std::string GetStringValue(const char* name_argument, size_t index);
    bool IsPositional();
    bool TypeDefinition();

private:
    std::string name_;
    std::string name_positional_;
    std::unordered_map<std::string_view, BaseArgument*> arguments_;
	std::unordered_map<char, BaseArgument*> arguments_short_;

};

}