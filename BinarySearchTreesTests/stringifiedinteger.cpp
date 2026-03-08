#include "stringifiedinteger.h"

#include <algorithm>
#include <cassert>
#include <regex>
#include <string_view>
#include <unordered_map>

static constexpr std::string_view c_ZeroStr{"Z"};
static constexpr std::string_view c_MinusInfiniteStr{"N_"};
static constexpr std::string_view c_PlusInfiniteStr{"N"};

static constexpr std::string_view c_SingleCharRegexStr{"[a-iA-IzZ]{1}"};
static constexpr std::string_view c_MultipleCharsRegexStr{"([zZ]*)?([a-iA-I]{1}[a-iA-IzZ]{0,9})?_?"};

static constexpr std::string_view c_IntMinExceedingValueStr{
    "2147483649"}; // absolute value of the first negative value which is considered out-of-bounds for a 32-bit integer
static constexpr std::string_view c_IntMaxExceedingValueStr{
    "2147483648"}; // first positive value which is considered out-of-bounds for a 32-bit integer

static constexpr size_t c_MaxPositiveIntDigitsCount{10};
static constexpr size_t c_MaxNegativeIntDigitsCount{11};

namespace TestUtils
{
namespace
{
char mapToIntChar(char stringifiedIntCh)
{
    static const std::unordered_map<char, char> c_CharConversionMap{
        {'Z', '0'}, {'A', '1'}, {'B', '2'}, {'C', '3'}, {'D', '4'}, {'E', '5'}, {'F', '6'},
        {'G', '7'}, {'H', '8'}, {'I', '9'}, {'z', '0'}, {'a', '1'}, {'b', '2'}, {'c', '3'},
        {'d', '4'}, {'e', '5'}, {'f', '6'}, {'g', '7'}, {'h', '8'}, {'i', '9'}, {'_', '-'}};

    return c_CharConversionMap.at(stringifiedIntCh);
}

std::string parseInputString(const std::string& inputStr)
{
    const bool c_IsNegative{inputStr.ends_with('_')};
    std::string result{c_IsNegative ? c_MinusInfiniteStr : c_PlusInfiniteStr};

    for (;;)
    {
        const size_t c_InputStringLength{inputStr.size()};

        if (c_InputStringLength == 0)
        {
            break;
        }

        if (c_InputStringLength == 1)
        {
            const std::regex c_SingleCharRe{c_SingleCharRegexStr.data()};

            if (std::regex_match(inputStr, c_SingleCharRe))
            {
                result = toupper(inputStr[0]);
            }

            break;
        }

        const std::regex c_MultipleCharRe{c_MultipleCharsRegexStr.data()};

        if (!std::regex_match(inputStr, c_MultipleCharRe))
        {
            break;
        }

        if (inputStr.starts_with('Z') || inputStr.starts_with('z'))
        {
            const auto it{
                std::find_if(inputStr.cbegin(), inputStr.cend(), [](const auto& c) { return c != 'Z' && c != 'z'; })};

            if (it == inputStr.cend() || *it == '_')
            {
                result = c_ZeroStr;
            }
            else
            {
                const size_t c_Position{static_cast<size_t>(std::distance(inputStr.cbegin(), it))};
                result = inputStr.substr(c_Position);
            }
        }
        else
        {
            result = inputStr;
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
            const std::string c_ExceedingValueToCompareTo{c_IsNegative ? c_IntMinExceedingValueStr
                                                                       : c_IntMaxExceedingValueStr};
            const bool c_IsWithinBounds{std::lexicographical_compare(absValue.cbegin(), absValue.cend(),
                                                                     c_ExceedingValueToCompareTo.cbegin(),
                                                                     c_ExceedingValueToCompareTo.cend())};

            if (!c_IsWithinBounds)
            {
                result = c_IsNegative ? c_MinusInfiniteStr : c_PlusInfiniteStr;
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
    : m_Str{c_ZeroStr}
{
}

TestUtils::StringifiedInteger::StringifiedInteger(const std::string& str)
    : m_Str{parseInputString(str)}
{
}

TestUtils::StringifiedInteger::StringifiedInteger(const char* str)
    : StringifiedInteger{str ? std::string{str} : std::string{}}
{
}

std::strong_ordering TestUtils::operator<=>(const TestUtils::StringifiedInteger& first,
                                            const TestUtils::StringifiedInteger& second)
{
    std::strong_ordering result{std::strong_ordering::equal};

    for (;;)
    {
        const std::optional<int> c_FirstIntValue{first._convertToInteger()};

        if (!c_FirstIntValue.has_value())
        {
            if (first.m_Str == c_MinusInfiniteStr)
            {
                if (second.m_Str != c_MinusInfiniteStr)
                {
                    result = std::strong_ordering::less;
                }

                break;
            }

            if (second.m_Str != c_PlusInfiniteStr)
            {
                result = std::strong_ordering::greater;
            }

            break;
        }

        const std::optional<int> c_SecondIntValue{second._convertToInteger()};

        if (!c_SecondIntValue.has_value())
        {
            result = second.m_Str == c_MinusInfiniteStr ? std::strong_ordering::greater : std::strong_ordering::less;
            break;
        }

        result = *c_FirstIntValue <=> *c_SecondIntValue;
        break;
    }

    return result;
}

const std::string& TestUtils::StringifiedInteger::str() const
{
    return m_Str;
}

std::ostream& TestUtils::operator<<(std::ostream& out, const TestUtils::StringifiedInteger& stringifiedInt)
{
    out << stringifiedInt.m_Str;
    return out;
}

std::optional<int> TestUtils::StringifiedInteger::_convertToInteger() const
{
    std::optional<int> result;

    if (m_Str != c_MinusInfiniteStr && m_Str != c_PlusInfiniteStr)
    {
        std::string stringToConvert{m_Str};

        if (m_Str.ends_with('_'))
        {
            std::rotate(stringToConvert.rbegin(), stringToConvert.rbegin() + 1, stringToConvert.rend());
        }

        std::transform(stringToConvert.cbegin(), stringToConvert.cend(), stringToConvert.begin(),
                       [](char ch) { return mapToIntChar(ch); });
        result = std::stoi(stringToConvert);
    }

    return result;
}
