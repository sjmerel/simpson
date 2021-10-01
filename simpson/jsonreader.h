#pragma once

#include "simpson/jsonvalue.h"
#include <iostream>

namespace Simpson 
{


class Tokenizer;

class JsonReader
{
public:
    JsonReader(std::istream&);
    ~JsonReader();

    bool read(JsonValue&);

    int line() const;
    int column() const;
    int pos() const;

private:
    JsonReader& operator=(const JsonReader&) = delete;
    JsonReader(const JsonReader&) = delete;

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


} // namespace Simpson
