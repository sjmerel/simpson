#pragma once
#include "tokentype.h"
#include <string>

struct Token
{
    TokenType type;
    std::string value;
};
