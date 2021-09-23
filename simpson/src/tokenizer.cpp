#include "simpson/src/tokenizer.h"
#include <iostream>

// see: https://www.json.org/json-en.html

namespace Simpson 
{

namespace
{
    // not using std::isspace() because it is more permissive
    bool isSpace(char c)
    {
        return c == 0x20 ||  // space
               c == 0x09 ||  // tab (\t)
               c == 0x0a ||  // linefeed (\n)
               c == 0x0d;    // carriage return (\r)
    }

    // not using std::isdigit() because it can depend on locale on Windows!
    bool isDigit(char c)
    {
        return c >= '0' && c <= '9';
    }
}

////////////////////////////////////////

Tokenizer::Tokenizer(std::istream& stream) :
    m_stream(stream)
{
}

bool Tokenizer::advance()
{
    while (isSpace(m_stream.peek()))
    {
        if (m_stream.peek() == '\n')
        {
            ++m_line;
            m_lineStart = (int) m_stream.tellg() + 1;
        }
        m_stream.ignore();
    }
    if (m_stream.eof()) { return false; }

    char c = m_stream.get();
    m_token.value = c;

    // TODO handle comments ("//", "/*", etc) as an extension?

    if (c == '"')
    {
        m_token.type = TokenType::STRING;
        readString();
    }
    else if (isDigit(c) || c == '-')
    {
        m_token.type = TokenType::NUMBER;
        readNumber();
    }
    else if (c == 't')
    {
        m_token.type = TokenType::BOOLEAN;
        readLiteral("true");
    }
    else if (c == 'f')
    {
        m_token.type = TokenType::BOOLEAN;
        readLiteral("false");
    }
    else if (c == 'n')
    {
        m_token.type = TokenType::NULL_;
        readLiteral("null");
    }
    else if (c == '[')
    {
        m_token.type = TokenType::ARRAY_START;
    }
    else if (c == ']')
    {
        m_token.type = TokenType::ARRAY_END;
    }
    else if (c == '{')
    {
        m_token.type = TokenType::OBJECT_START;
    }
    else if (c == '}')
    {
        m_token.type = TokenType::OBJECT_END;
    }
    else if (c == ':')
    {
        m_token.type = TokenType::COLON;
    }
    else if (c == ',')
    {
        m_token.type = TokenType::COMMA;
    }
    else
    {
        m_fail = true;
    }

    return !fail() && !eof();
}

bool Tokenizer::eof() const
{
    return m_stream.eof();
}

bool Tokenizer::fail() const
{
    return m_stream.fail() || m_fail;
}

void Tokenizer::readLiteral(const std::string& literalValue)
{
    while (!m_fail && m_token.value.size() < literalValue.size())
    {
        char c = m_stream.get();
        if (literalValue[m_token.value.size()] == c)
        {
            m_token.value += c;
        }
        else
        {
            m_fail = true;
            break;
        }
    }
}

void Tokenizer::readNumber()
{
    // TODO optionally handle NaN/Infinity/-Infinity?
    char c;
    while ((isDigit(c = m_stream.peek())) || c == '-' || c == '+' || c == 'e' || c == 'E' || c == '.')
    {
        m_token.value += m_stream.get();
    }
}

void Tokenizer::readUnicodeEscape()
{
    char buf[4];
    m_stream.read(buf, 4);
    if (m_stream.eof())
    {
        m_fail = true;
        return;
    }

    int value = 0;
    for (int i = 0; i < 4; ++i)
    {
        char c = buf[i];
        int v = 0;
        if (c >= '0' && c <= '9')
        {
            v = c - '0';
        }
        else if (c >= 'a' && c <= 'f')
        {
            v = 10 + (c - 'a');
        }
        else if (c >= 'A' && c <= 'F')
        {
            v = 10 + (c - 'A');
        }
        else
        {
            m_fail = true;
            break;
        }

        value |= (v << (3-i)*4);
    }

    // UTF-8 encode
    if (value <= 0x7f)
    {
        m_token.value += (char) value;
    }
    else if (value <= 0x7ff)
    {
        m_token.value += (char) (0xc0 | (value >> 6));
        m_token.value += (char) (0x80 | (value & 0x3f));
    }
    else // if (value <= 0xffff)
    {
        m_token.value += (char) (0xe0 | (value >> 12));
        m_token.value += (char) (0x80 | ((value >> 6) & 0x3f));
        m_token.value += (char) (0x80 | (value & 0x3f));
    }
}

void Tokenizer::readString()
{
    char c = 0;
    m_token.value.clear(); // skip quote

    bool escaping = false;
    while (!m_fail && !m_stream.eof())
    {
        c = m_stream.get();
        if (escaping)
        {
            switch (c)
            {
                case '"': 
                case '\\':
                case '/':
                    m_token.value += c; 
                    break;

                case 'b':
                    m_token.value += '\b';
                    break;
                case 'f':
                    m_token.value += '\f';
                    break;
                case 'n':
                    m_token.value += '\n';
                    break;
                case 'r':
                    m_token.value += '\r';
                    break;
                case 't':
                    m_token.value += '\t';
                    break;

                case 'u': 
                    readUnicodeEscape();
                    break;

                default:
                    m_fail = true;
                    break;
            }
            escaping = false;
        }
        else if (c == '"')
        {
            // end of string
            break;
        }
        else
        {
            if (c == '\\')
            {
                // start an escape sequence
                escaping = true;
            }
            else if (c >= 0x0 && c <= 0x1f)
            {
                // check for unescaped U+0000 through U+001F
                m_fail = true;
            }
            else
            {
                m_token.value += c;
            }
        }
    }
    if (c != '"')
    {
        // we hit EOF before the string was closed
        m_fail = true;
    }
}

} // namespace Simpson
