#include "stringifiedinteger.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <string_view>
#include <unordered_map>

static constexpr std::string_view c_ZeroValue{"Z"};
static constexpr std::string_view c_MinusInfinite{"N_"};
static constexpr std::string_view c_PlusInfinite{"N"};

static constexpr std::string_view c_SingleCharRegexStr{"[a-iA-IzZ]{1}"};
static constexpr std::string_view c_MultipleCharsRegexStr{"([zZ]*)?([a-iA-I]{1}[a-iA-IzZ]{0,9})?_?"};

static constexpr std::string_view c_IntMinExceedingValue{
    "2147483649"}; // absolute value of the first negative value which is considered out-of-bounds for a 32-bit integer
static constexpr std::string_view c_IntMaxExceedingValue{
    "2147483648"}; // first positive value which is considered out-of-bounds for a 32-bit integer

static constexpr size_t c_MaxPositiveIntDigitsCount{10};
static constexpr size_t c_MaxNegativeIntDigitsCount{11};

namespace TestUtils
{
namespace
{
char mapToIntChar(char value)
{
    static const std::unordered_map<char, char> c_CharConversionMap{
        {'Z', '0'}, {'A', '1'}, {'B', '2'}, {'C', '3'}, {'D', '4'}, {'E', '5'}, {'F', '6'},
        {'G', '7'}, {'H', '8'}, {'I', '9'}, {'z', '0'}, {'a', '1'}, {'b', '2'}, {'c', '3'},
        {'d', '4'}, {'e', '5'}, {'f', '6'}, {'g', '7'}, {'h', '8'}, {'i', '9'}, {'_', '-'}};

    return c_CharConversionMap.at(value);
}

std::string parseInputString(const std::string& inputString)
{
    const bool c_IsNegative{inputString.ends_with('_')};
    std::string result{c_IsNegative ? c_MinusInfinite : c_PlusInfinite};

    for (;;)
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
                result = toupper(inputString[0]);
            }

            break;
        }

        const std::regex c_MultipleCharRe{c_MultipleCharsRegexStr.data()};

        if (!std::regex_match(inputString, c_MultipleCharRe))
        {
            break;
        }

        if (inputString.starts_with('Z') || inputString.starts_with('z'))
        {
            const auto it{std::find_if(inputString.cbegin(), inputString.cend(),
                                       [](const auto& c) { return c != 'Z' && c != 'z'; })};

            if (it == inputString.cend() || *it == '_')
            {
                result = c_ZeroValue;
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

        const size_t c_ResultLength{result.size()};
        const bool c_HasMaxDigitsCount{c_IsNegative ? (c_ResultLength == c_MaxNegativeIntDigitsCount)
                                                    : (c_ResultLength == c_MaxPositiveIntDigitsCount)};

        // handle the case when the resulting number would cause exceeding the min/max 32-bit int value
        if (c_HasMaxDigitsCount)
        {
            std::string absValue{c_IsNegative ? result.substr(0, c_ResultLength - 1) : result};
            std::transform(absValue.cbegin(), absValue.cend(), absValue.begin(),
                           [](char ch) { return mapToIntChar(ch); });
            const std::string c_ExceedingValueToCompareTo{c_IsNegative ? c_IntMinExceedingValue
                                                                       : c_IntMaxExceedingValue};
            const bool c_IsWithinBounds{std::lexicographical_compare(absValue.cbegin(), absValue.cend(),
                                                                     c_ExceedingValueToCompareTo.cbegin(),
                                                                     c_ExceedingValueToCompareTo.cend())};

            if (!c_IsWithinBounds)
            {
                result = c_IsNegative ? c_MinusInfinite : c_PlusInfinite;
                break;
            }
        }

        std::transform(result.cbegin(), result.cend(), result.begin(), [](char ch) { return toupper(ch); });
        break;
    }

    return result;
}
} // namespace
} // namespace TestUtils

TestUtils::StringifiedInteger::StringifiedInteger()
    : m_Value{c_ZeroValue}
{
}

TestUtils::StringifiedInteger::StringifiedInteger(const std::string& str)
    : m_Value{parseInputString(str)}
{
}

TestUtils::StringifiedInteger& TestUtils::StringifiedInteger::operator=(const char* str)
{
    this->m_Value = parseInputString(str ? std::string{str} : std::string{});
    return *this;
}

std::strong_ordering TestUtils::operator<=>(const TestUtils::StringifiedInteger& first,
                                            const TestUtils::StringifiedInteger& second)
{
    std::strong_ordering result{std::strong_ordering::equal};

    for (;;)
    {
        const std::optional<int> c_IntValue{first._convertToInteger()};

        if (!c_IntValue.has_value())
        {
            if (first.m_Value == c_MinusInfinite)
            {
                if (second.m_Value != c_MinusInfinite)
                {
                    result = std::strong_ordering::less;
                }

                break;
            }

            if (second.m_Value != c_PlusInfinite)
            {
                result = std::strong_ordering::greater;
            }

            break;
        }

        const std::optional<int> c_OtherIntValue{second._convertToInteger()};

        if (!c_OtherIntValue.has_value())
        {
            result = second.m_Value == c_MinusInfinite ? std::strong_ordering::greater : std::strong_ordering::less;
            break;
        }

        result = *c_IntValue <=> *c_OtherIntValue;
        break;
    }

    return result;
}

std::string TestUtils::StringifiedInteger::getValue() const
{
    return m_Value;
}

std::ostream& TestUtils::operator<<(std::ostream& out, const TestUtils::StringifiedInteger& stringifiedInt)
{
    out << stringifiedInt.m_Value;
    return out;
}

std::optional<int> TestUtils::StringifiedInteger::_convertToInteger() const
{
    std::optional<int> result;

    if (m_Value != c_MinusInfinite && m_Value != c_PlusInfinite)
    {
        std::string stringToConvert{m_Value};

        if (m_Value.ends_with('_'))
        {
            std::rotate(stringToConvert.rbegin(), stringToConvert.rbegin() + 1, stringToConvert.rend());
        }

        std::transform(stringToConvert.cbegin(), stringToConvert.cend(), stringToConvert.begin(),
                       [](char ch) { return mapToIntChar(ch); });
        result = std::stoi(stringToConvert);
    }

    return result;
}
