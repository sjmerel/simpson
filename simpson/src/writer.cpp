#include "simpson/writer.h"
#include <iomanip>

namespace Simpson 
{

namespace
{
    std::string escape(const std::string& s)
    {
        std::string result = s;
        for (int i = 0; i < (int) result.length(); ++i)
        {
            std::string repl;
            switch (result[i])
            {
                case '"':
                    repl = "\\\"";
                    break;
                case '\\':
                    repl = "\\\\";
                    break;
                case '\b':
                    repl = "\\b";
                    break;
                case '\f':
                    repl = "\\f";
                    break;
                case '\n':
                    repl = "\\n";
                    break;
                case '\r':
                    repl = "\\r";
                    break;
                case '\t':
                    repl = "\\t";
                    break;

                // TODO: optionally escape forward slash
                // TODO: optionally escape unicode chars > U+007f

                default: 
                    break;
            }
            if (!repl.empty())
            {
                result.replace(i++, 1, repl);
            }
        }
        return result;
    }
}

////////////////////////////////////////

Writer::Writer(std::ostream& stream) :
    m_stream(stream)
{
}

void Writer::write(const JsonValue& value)
{
    writeImpl(value);
}

void Writer::setIndent(int spaces) 
{
    m_indent = std::string(spaces, ' ');
}

////////////////////////////////////////

void Writer::writeIndent(int level)
{
    for (int i = 0; i < level; ++i)
    {
        m_stream << m_indent;
    }
}

void Writer::writeImpl(const JsonValue& value, int level, const std::string& prefix)
{
    writeIndent(level);
    m_stream << prefix;
    switch (value.type())
    {
        case JsonValue::Type::Boolean:
            m_stream << (value.boolean() ? "true" : "false");
            break;

        case JsonValue::Type::Number:
            m_stream << std::setprecision(17) << value.number();
            break;

        case JsonValue::Type::String:
            m_stream << "\"" << escape(value.string()) << "\"";
            break;

        case JsonValue::Type::Null:
            m_stream << "null";
            break;

        case JsonValue::Type::Array:
            if (value.size() == 0)
            {
                m_stream << "[]";
            }
            else
            {
                m_stream << "[" << std::endl;
                for (int i = 0; i < value.size(); ++i)
                {
                    writeImpl(value.get(i), level+1);
                    if (i < value.size()-1)
                    {
                        m_stream << ",";
                    }
                    m_stream << std::endl;
                }
                writeIndent(level);
                m_stream << "]";
            }
            break;

        case JsonValue::Type::Object:
            if (value.size() == 0)
            {
                m_stream << "{}";
            }
            else
            {
                m_stream << "{" << std::endl;
                for (int i = 0; i < value.size(); ++i)
                {
                    const std::string& key = value.key(i);
                    const JsonValue& keyValue = value.get(key);
                    std::string prefix = "\"" + key + "\": ";
                    writeImpl(keyValue, level+1, prefix);
                    if (i < value.size()-1)
                    {
                        m_stream << ",";
                    }
                    m_stream << std::endl;
                }
                writeIndent(level);
                m_stream << "}";
            }
            break;
    }
}

} // namespace Simpson
