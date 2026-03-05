#include "stringifiedinteger.h"

#include <regex>
#include <string_view>

static constexpr std::string_view c_ZeroValue{"Z"};
static constexpr std::string_view c_MinusInfinite{"N_"};
static constexpr std::string_view c_PlusInfinite{"N"};

static constexpr std::string_view c_SingleCharRegexStr{"[a-iA-IzZ]{1}"};
static constexpr std::string_view c_MultipleCharsRegexStr{"([zZ]*)?([a-iA-I]{1}[a-iA-IzZ]{0,9})?_?"};

namespace TestUtils
{
namespace
{
std::string parseInputString(const std::string& inputString)
{
    std::string result{inputString.ends_with('_') ? "N_" : "N"};

    do
    {
        const size_t c_Length{inputString.size()};

        if (c_Length == 0)
        {
            break;
        }

        if (c_Length == 1)
        {
            const std::regex c_SingleCharRe{c_SingleCharRegexStr.data()};

            if (std::regex_match(inputString, c_SingleCharRe))
            {
                result = inputString;
            }

            break;
        }

        const std::regex c_MultipleCharRe{c_MultipleCharsRegexStr.data()};

        if (!std::regex_match(inputString, c_MultipleCharRe))
        {
            break;
        }

        if (inputString.starts_with("Z"))
        {
            const auto it{
                std::find_if(inputString.cbegin(), inputString.cend(), [](const auto& c) { return c != 'Z'; })};

            if (it == inputString.cend() || *it == '_')
            {
                result = "Z";
            }
            else
            {
                const size_t c_Position{static_cast<size_t>(std::distance(inputString.cbegin(), it))};
                result = inputString.substr(c_Position);
            }
        }
        else
        {
            result = inputString;
        }
    } while (false);

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
