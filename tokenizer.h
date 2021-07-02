#pragma once

#include "token.h"
#include <string>
#include <iostream>

class Tokenizer
{
public:
    Tokenizer(std::istream&);

    bool advance();
    bool eof() const;
    bool fail() const;
    const Token& getToken() { return m_token; }

private:
    std::istream& m_stream;
    bool m_fail = false;
    Token m_token;

    void readLiteral(const std::string& literalValue);
};

