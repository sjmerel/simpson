#pragma once

#include "simpson/jsonvalue.h"
#include <iostream>

namespace Simpson 
{


class JsonWriter
{
public:
    JsonWriter(std::ostream&);

    void setIndent(int);

    void write(const JsonValue&);

private:
    std::ostream& m_stream;
    std::string m_indent = "  ";

    void writeIndent(int level);
    void writeImpl(const JsonValue&, int level = 0, const std::string& prefix = "");

    JsonWriter& operator=(const JsonWriter&) = delete;
    JsonWriter(const JsonWriter&) = delete;
};


} // namespace Simpson
