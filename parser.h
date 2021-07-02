#pragma once

#include "tokenizer.h"
#include <iostream>

class Parser
{
public:
    Parser(std::istream&);

    bool parse();

private:
    Tokenizer m_tokenizer;
    bool m_fail = false;

    int m_level = 0;

    bool parseBoolean();
    bool parseNumber();
    bool parseString();
    bool parseNull();
    bool parseArray();
    bool parseObject();
    bool parseValue();
    void printIndented(const std::string&);
};

