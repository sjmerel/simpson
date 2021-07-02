#include "tokenizer.h"
#include <iostream>

Tokenizer::Tokenizer(std::istream& stream) :
    m_stream(stream)
{
}

bool Tokenizer::advance()
{
    while (std::isspace(m_stream.peek()))
    {
        m_stream.ignore();
    }
    if (m_stream.eof()) { return false; }

    char c = m_stream.get();
    //std::cerr << " c: " << (int) c << std::endl;

    if (c == '"')
    {
        // read string
        // TODO escaping etc
        m_token.type = TokenType::STRING;
        m_token.value.clear();
        while ((c = m_stream.get()) != '"' && !m_stream.eof())
        {
            m_token.value += c;
        }
        if (c != '"')
        {
            m_fail = true;
        }
    }
    else if (std::isdigit(c) || c == '-')
    {
        // read number
        // TODO floats
        m_token.type = TokenType::NUMBER;
        m_token.value = c;
        while (std::isdigit(m_stream.peek()))
        {
            m_token.value += m_stream.get();
        }
    }
    else if (c == 't')
    {
        m_token.type = TokenType::BOOLEAN;
        m_token.value = c;
        readLiteral("true");
    }
    else if (c == 'f')
    {
        m_token.type = TokenType::BOOLEAN;
        m_token.value = c;
        readLiteral("false");
    }
    else if (c == 'n')
    {
        m_token.type = TokenType::NULL_;
        m_token.value = c;
        readLiteral("null");
    }
    else if (c == '[')
    {
        m_token.type = TokenType::ARRAY_START;
        m_token.value = c;
    }
    else if (c == ']')
    {
        m_token.type = TokenType::ARRAY_END;
        m_token.value = c;
    }
    else if (c == '{')
    {
        m_token.type = TokenType::OBJECT_START;
        m_token.value = c;
    }
    else if (c == '}')
    {
        m_token.type = TokenType::OBJECT_END;
        m_token.value = c;
    }
    else if (c == ':')
    {
        m_token.type = TokenType::COLON;
        m_token.value = c;
    }
    else if (c == ',')
    {
        m_token.type = TokenType::COMMA;
        m_token.value = c;
    }
    else
    {
        m_fail = true;
    }

    // std::cerr << "token type: " << (int) m_token.type << std::endl;

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
    while (m_token.value.size() < literalValue.size())
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

