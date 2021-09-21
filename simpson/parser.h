#pragma once

#include "simpson/jsonvalue.h"
#include <iostream>

class Tokenizer;

class Parser
{
public:
    Parser(std::istream&);
    ~Parser();

    bool parse(JsonValue&);

    int line() const;
    int column() const;
    int pos() const;

private:
    Parser& operator=(const Parser&);
    Parser(const Parser&);

    Tokenizer* m_tokenizer;
    bool m_fail = false;

    bool parseBoolean(JsonValue&);
    bool parseNumber(JsonValue&);
    bool parseString(JsonValue&);
    bool parseString(std::string&);
    bool parseNull(JsonValue&);
    bool parseArray(JsonValue&);
    bool parseObject(JsonValue&);
    bool parseValue(JsonValue&);
    bool fail() const;
};
