#pragma once

#include <iostream>
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

   The minus sign ('-') is mapped to the '_' character which is now placed at the end of the string sequence
   - example 1: "BZA" is the equivalent for 201
   - example 2: "GAB_" is the equivalent for -702

   In addition to this the -infinite and +infinite values have been introduced:
   - "N" for +infinite
   - "N_" for -infinite

   Any invalid input string is mapped to either -infinite/+infinite depending on whether the last character
   from the sequence is '_' or not:
   - example 1: input string "BZ A" is stored as "N" within the StringifiedInteger object
   - example 2: input string "BZ1_" is stored as "N_" within the StringifiedInteger object
 */

namespace TestUtils
{
class StringifiedInteger
{
public:
    StringifiedInteger();
    StringifiedInteger(const std::string& str);
    StringifiedInteger(const char* str);

    friend std::strong_ordering operator<=>(const StringifiedInteger& first, const StringifiedInteger& second);
    bool operator==(const StringifiedInteger& other) const = default;

    const std::string& str() const;

    friend std::ostream& operator<<(std::ostream& out, const StringifiedInteger& stringifiedInt);

protected:
    std::optional<int> _convertToInteger() const;

private:
    std::string m_Str;
};

std::strong_ordering operator<=>(const StringifiedInteger&, const StringifiedInteger&);
std::ostream& operator<<(std::ostream&, const StringifiedInteger&);
} // namespace TestUtils
