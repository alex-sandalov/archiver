#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <cstring>
#include <cstdint>
#include <iostream>
#include <charconv>
#include <string_view>

const int kDefaultValueMinArgs = 1;

class BaseArgument {
public:
	bool has_default_value;
    bool has_store_value;
    bool has_multi_value;
    bool is_positional;
    bool has_value;
	char short_name;
	std::string_view long_name;
	std::string_view description;

	BaseArgument() = default;
	BaseArgument(const char short_name, const char* long_name, const char* description);
	virtual ~BaseArgument() = default;

private:
	
};

template <typename Type>
class Argument : public BaseArgument {
public:
	Argument() = default;
    ~Argument() = default;
	Argument(const char short_name, const char* long_name, const char* description);
	Argument& Positional();
    Argument& Default(Type value);
    Argument& StoreValue(Type& link_value);
    Argument& MultiValue(int min_args_counts = kDefaultValueMinArgs);
    Argument& StoreValues(std::vector<Type>& link_value);
    Argument& SetValue(Type value);
    Type GetValue(int index = kDefaultValueMinArgs);
    size_t GetSizeMultiValue();

private:
	int min_args_counts_;
	Type value_;
    Type default_value_;
    Type* store_value_;
    std::vector<Type> multi_value_;
    std::vector<Type>* multi_store_value_;

};