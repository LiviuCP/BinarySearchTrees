#pragma once

#include <string>

class StringifiedInteger
{
public:
    StringifiedInteger();
    StringifiedInteger(const std::string& value);

private:
    std::string m_Value;
};
