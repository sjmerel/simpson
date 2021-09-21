#pragma once
#include "simpson/src/tokentype.h"
#include <string>

struct Token
{
    TokenType type;
    std::string value;
};
