#pragma once

#include "simpson/jsonvalue.h"
#include <iostream>

namespace Simpson 
{


class Writer
{
public:
    Writer(std::ostream&);

    void setIndent(int);

    void write(const JsonValue&);

private:
    std::ostream& m_stream;
    std::string m_indent = "  ";

    void writeIndent(int level);
    void writeImpl(const JsonValue&, int level = 0, const std::string& prefix = "");

    Writer& operator=(const Writer&) = delete;
    Writer(const Writer&) = delete;
};


} // namespace Simpson
