#include <Argument.h>

BaseArgument::BaseArgument(const char short_name, const char* long_name, const char* description)
	: has_default_value(false)
	, has_store_value(false)
    , has_multi_value(false)
    , is_positional(false)
    , has_value(false)
	, short_name(short_name)
	, long_name(long_name)
	, description(description)
{}

template <typename Type>
Argument<Type>::Argument(const char short_name, const char* long_name, const char* description)
	: BaseArgument(short_name, long_name, description)
{}

template <typename Type>
Argument<Type>& Argument<Type>::Default(Type value) {
    has_default_value = true;
    has_value = true;
    default_value_ = value;
    value_ = value;
    return *this;
}

template <typename Type>
Argument<Type>& Argument<Type>::StoreValue(Type& link_value) {
    store_value_ = &link_value;
    has_store_value = true;
    return *this;
}

template <typename Type>
Argument<Type>& Argument<Type>::StoreValues(std::vector<Type>& link_value) {
    multi_store_value_ = &link_value;
    has_store_value = true;
    return *this;
}

template <typename Type>
Argument<Type>& Argument<Type>::MultiValue(int min_args_counts) {
    min_args_counts_ = min_args_counts;
    has_multi_value = true;
    return *this;
}

template <typename Type>
Argument<Type>& Argument<Type>::Positional() {
    is_positional = true;
    return *this;
}

template <typename Type>
Argument<Type>& Argument<Type>::SetValue(Type value) {
    has_value = true;
    if (has_multi_value) {
        if (has_store_value) {
            multi_store_value_->push_back(value);
        }
        multi_value_.push_back(value);
        return *this;
    }
    if (has_store_value) {
        *store_value_ = value;
    }
    value_ = value;
    return *this;
}

template <typename Type>
Type Argument<Type>::GetValue(int index) {
    if (has_multi_value && index < multi_value_.size()) {
        return multi_value_[index];
    }
    return value_;
}

template <typename Type>
size_t Argument<Type>::GetSizeMultiValue() {
    return multi_value_.size();
}

template struct Argument<int>;
template struct Argument<bool>;
template struct Argument<std::string>;