#pragma once

#include "simpson/src/token.h"
#include <string>
#include <iostream>

namespace Simpson 
{

class Tokenizer
{
public:
    Tokenizer(std::istream&);

    bool advance();
    bool eof() const;
    bool fail() const;

    int line() const { return m_line; }
    int column() const { return (int) m_stream.tellg() - m_lineStart - 1; }
    int pos() const { return (int) m_stream.tellg(); }

    const Token& getToken() { return m_token; }

private:
    std::istream& m_stream;
    bool m_fail = false;
    int m_line = 0;
    int m_lineStart = 1;
    Token m_token;

    void readLiteral(const std::string& literalValue);
    void readNumber();
    void readString();
    void readUnicodeEscape();
};

} // namespace Simpson
