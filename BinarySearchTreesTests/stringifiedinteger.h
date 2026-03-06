#pragma once

#include <optional>
#include <string>

/* This is a class that can be used for testing binary search trees.
   It represents an integer that is rewritten using characters instead of numeric digits.

   Each digit is mapped to a character as follows:
   - 0: 'Z'
   - 1: 'A'
   - 2: 'B'
   ...
   - 9: 'I'

   The - sign is mapped to the '_' character which is now placed at the end of the string sequence
   - example 1: "BZA" is the equivalent for 201
   - example 2: "GAB_" is the equivalent for -702

   In addition to this the -infinite and +infinite values are being introduced:
   - "N" for +infinite
   - "N_" for -infinite

   Any invalid string is mapped to either -infinite/+infinite depending on whether the last character in the sequence is
   '_' or not.
 */

namespace TestUtils
{
class StringifiedInteger
{
public:
    StringifiedInteger();
    StringifiedInteger(const std::string& value);

    friend std::strong_ordering operator<=>(const StringifiedInteger& first, const StringifiedInteger& second);
    bool operator==(const StringifiedInteger& other) const = default;

    std::string getValue() const;

protected:
    std::optional<int> _getIntValue() const;

private:
    std::string m_Value;
};

std::strong_ordering operator<=>(const StringifiedInteger&, const StringifiedInteger&);

} // namespace TestUtils
