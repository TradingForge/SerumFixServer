#pragma once
#include <string>

namespace instruments
{
    __uint128_t atouint128_t(const std::string& in)
    {
        __uint128_t res = 0;
        for (const char& c : in)
        {
            if (not std::isdigit(c)) 
                throw std::string("Non-numeric character: ");
            res *= 10;
            res += c - '0';
        }
        return res;
    }   
} // namespace instruments