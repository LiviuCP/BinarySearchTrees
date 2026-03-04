#pragma once

#include <string>

class StringifiedInteger
{
public:
    StringifiedInteger();
    StringifiedInteger(const std::string& value);

    std::string getValue() const;

private:
    std::string m_Value;
};
