#pragma once
#include "simpson/src/tokentype.h"
#include <string>

namespace Simpson 
{

struct Token
{
    TokenType type;
    std::string value;
};

} // namespace Simpson
