#include <ArgParser.h>

namespace ArgumentParser {

ArgParser::ArgParser(const char* name)
    : name_(name)
{}

Argument<bool>& ArgParser::AddFlag(const char* long_name) {
    return ArgParser::AddFlag(' ', long_name, "none description");
}

Argument<bool>& ArgParser::AddFlag(const char short_name, const char* long_name) {
    return ArgParser::AddFlag(short_name, long_name, "none description");
}

Argument<bool>& ArgParser::AddFlag(const char* long_name, const char* description) {
    return ArgParser::AddFlag(' ', long_name, description);
}

Argument<bool>& ArgParser::AddFlag(const char short_name, const char* long_name, const char* description) {
    this->arguments_[long_name] = new Argument<bool>(short_name, long_name, description);
    auto argument = dynamic_cast<Argument<bool>*>(arguments_[long_name]);
    argument->SetValue(false);
    if (!std::isspace(short_name)) {
        arguments_short_[short_name] = arguments_[long_name];
    }
    return *argument;
}

Argument<int>& ArgParser::AddIntArgument(const char* long_name) {
    return ArgParser::AddIntArgument(' ', long_name, "none description");
}

Argument<int>& ArgParser::AddIntArgument(const char short_name, const char* long_name) {
    return ArgParser::AddIntArgument(short_name, long_name, "none description");
}

Argument<int>& ArgParser::AddIntArgument(const char* long_name, const char* description) {
    return ArgParser::AddIntArgument(' ', long_name, description);
}

Argument<int>& ArgParser::AddIntArgument(const char short_name, const char* long_name, const char* description) {
    arguments_[long_name] = new Argument<int>(short_name, long_name, description);
	auto argument = dynamic_cast<Argument<int>*>(arguments_[long_name]);
    if (!std::isspace(short_name)) {
        arguments_short_[short_name] = arguments_[long_name] ;
    }
    return *argument;
}

Argument<std::string>& ArgParser::AddStringArgument(const char* long_name) {
    return ArgParser::AddStringArgument(' ', long_name, "none description");
}

Argument<std::string>& ArgParser::AddStringArgument(char short_name, const char* long_name) {
    return ArgParser::AddStringArgument(short_name, long_name, "none description");
}

Argument<std::string>& ArgParser::AddStringArgument(const char* long_name, const char* description) {
    return ArgParser::AddStringArgument(' ', long_name, description);
}

Argument<std::string>& ArgParser::AddStringArgument(char short_name, const char* long_name, const char* description) {
    arguments_[long_name] = new Argument<std::string>(short_name, long_name, description);
	auto argument = dynamic_cast<Argument<std::string>*>(arguments_[long_name]);
    if (!std::isspace(short_name)) {
        arguments_short_[short_name] = arguments_[long_name];
    }
    return *argument;
}

bool ArgParser::GetFlag(const char* name_argument) {
    return dynamic_cast<Argument<bool>*>(arguments_[name_argument])->GetValue();
}

size_t ArgParser::GetIntValue(const char* name_argument) {
    return dynamic_cast<Argument<int>*>(arguments_[name_argument])->GetValue();
}

size_t ArgParser::GetIntValue(const char* name_argument, size_t index) {
    return dynamic_cast<Argument<int>*>(arguments_[name_argument])->GetValue(index);
}

std::string ArgParser::GetStringValue(const char* name_argument, size_t index) {
    return dynamic_cast<Argument<std::string>*>(arguments_[name_argument])->GetValue(index);
}

std::string ArgParser::GetStringValue(const char* name_argument)  {
    return dynamic_cast<Argument<std::string>*>(arguments_[name_argument])->GetValue();
}

bool ArgParser::Parse(const char** argv, int argc) {
	bool is_value = true;
    bool is_positional = false;
    std::string name_positional;
 
    for (int i = 1; i < argc; ++i) {
        std::string_view argument = argv[i];
        size_t index_equals = argument.find('=');
        bool is_long_argument = argument.starts_with("--");

        std::string_view long_argument = argument.substr(argument.rfind('-') + 1, index_equals - argument.rfind('-') - 1);
        const char short_argument = long_argument[0];

        if (!(arguments_.contains(long_argument) || arguments_short_.contains(short_argument)) && !is_positional) {
            return false;
        } else if (!(arguments_.contains(long_argument) || arguments_short_.contains(short_argument))) {
            const char* argument_char = long_argument.data();
            int value = std::atoi(argument_char);
            if (Argument<int>* int_argument = dynamic_cast<Argument<int>*>(arguments_[name_positional])) {
                int_argument->SetValue(value);
            } else if (Argument<std::string>* string_argument = dynamic_cast<Argument<std::string>*>(arguments_[name_positional])) {
                string_argument->SetValue(argument_char);
            }
            continue;
        }

        long_argument = !is_long_argument ? arguments_short_[short_argument]->long_name : long_argument;

        if (Argument<std::string>* string_argument = dynamic_cast<Argument<std::string>*>(arguments_[long_argument])) {
            std::string value = is_long_argument ? std::string(argument.substr(index_equals + 1)) : argv[++i];
            string_argument->SetValue(value);
            if (string_argument->is_positional) {
                is_positional = true;
                name_positional = long_argument;
            }
        } else if (Argument<int>* int_argument = dynamic_cast<Argument<int>*>(arguments_[long_argument])) {
            int value;
            auto [ptr, ec] = std::from_chars(argument.substr(index_equals + 1).data(), argument.substr(index_equals + 1).data() + argument.size() - index_equals, value);
            int_argument->SetValue(value);
            if (int_argument->is_positional) {
                is_positional = true;
                name_positional = long_argument;
            }
        } else {
            Argument<bool>* bool_argument = dynamic_cast<Argument<bool>*>(arguments_[long_argument]);
            bool_argument->SetValue(true);
        }

    }
    return is_value;
}

ArgParser::~ArgParser() {
    for (const auto& [key, value] : arguments_) {
        delete value;
    }
}

}  // namespace ArgumentParser
