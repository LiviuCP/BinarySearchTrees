#include "stringifiedinteger.h"

#include <regex>
#include <string_view>

static constexpr std::string_view c_ZeroValue{"Z"};
static constexpr std::string_view c_MinusInfinite{"N_"};
static constexpr std::string_view c_PlusInfinite{"N"};

namespace TestUtils
{
namespace
{
std::string parseInputString(const std::string& inputString)
{
    std::string result{c_PlusInfinite};

    if (!inputString.empty())
    {
        const std::regex validNumberRe{"[zZ]*[a-iA-IzZ]{0,10}_?"};
        result = std::regex_match(inputString, validNumberRe) ? inputString : inputString.ends_with('_') ? "N_" : "N";
    }

    return result;
}
} // namespace
} // namespace TestUtils

TestUtils::StringifiedInteger::StringifiedInteger()
    : m_Value{c_ZeroValue}
{
}

TestUtils::StringifiedInteger::StringifiedInteger(const std::string& value)
    : m_Value{parseInputString(value)}
{
}

std::string TestUtils::StringifiedInteger::getValue() const
{
    return m_Value;
}
