#pragma once

namespace Simpson 
{

enum class TokenType
{
    STRING,
    NUMBER,
    BOOLEAN,
    NULL_,
    ARRAY_START,
    ARRAY_END,
    OBJECT_START,
    OBJECT_END,
    COLON,
    COMMA,
};

} // namespace Simpson
