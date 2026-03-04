#include "stringifiedinteger.h"

#include <string_view>

static constexpr std::string_view c_ZeroValue{"Z"};
static constexpr std::string_view c_MinusInfinite{"N_"};
static constexpr std::string_view c_PlusInfinite{"N"};

StringifiedInteger::StringifiedInteger()
    : m_Value{c_ZeroValue}
{
}

StringifiedInteger::StringifiedInteger(const std::string& value)
{
}

std::string StringifiedInteger::getValue() const
{
    return m_Value;
}
