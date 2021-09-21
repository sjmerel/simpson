#include "parser.h"
#include <iostream>
#include <sstream>

Parser::Parser(std::istream& stream) :
    m_tokenizer(stream)
{}

bool Parser::parse(JsonValue& value)
{
    m_tokenizer.advance();
    return parseValue(value);
}

int Parser::line() const
{
    return m_tokenizer.line();
}

int Parser::column() const
{
    return m_tokenizer.column();
}

int Parser::pos() const
{
    return m_tokenizer.pos();
}

////////////////////////////////////////

bool Parser::parseBoolean(JsonValue& value)
{
    if (m_tokenizer.getToken().type == TokenType::BOOLEAN)
    {
        value = m_tokenizer.getToken().value == "true" ? true : false;
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseNumber(JsonValue& value)
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::NUMBER)
    {
        // TODO handle NaN?
        std::istringstream stream(m_tokenizer.getToken().value);
        double d;
        stream >> d;
        if (!stream || !stream.eof())
        {
            m_fail = true;
            return false;
        }
        else
        {
            value = d;
            m_tokenizer.advance();
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Parser::parseString(JsonValue& value)
{
    std::string str;
    if (parseString(str))
    {
        value = str;
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseString(std::string& value)
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::STRING)
    {
        value = m_tokenizer.getToken().value;
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseNull(JsonValue& value)
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::NULL_)
    {
        value = JsonValue();
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseObject(JsonValue& value)
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::OBJECT_START)
    {
        int count = 0;
        m_tokenizer.advance();
        value = JsonValue(JsonValue::Type::Object);
        while (!fail() && m_tokenizer.getToken().type != TokenType::OBJECT_END)
        {
            if (count > 0)
            {
                if (m_tokenizer.getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                m_tokenizer.advance();
            }

            std::string key;
            JsonValue newValue;

            if (!parseString(key))
            {
                m_fail = true;
                return false;
            }
            if (m_tokenizer.getToken().type != TokenType::COLON)
            {
                m_fail = true;
                return false;
            }
            m_tokenizer.advance();
            if (parseValue(newValue))
            {
                value.set(key, newValue);
            }
            else
            {
                m_fail = true;
                return false;
            }
            ++count;
        }
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseArray(JsonValue& value)
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::ARRAY_START)
    {
        int count = 0;
        m_tokenizer.advance();
        value = JsonValue(JsonValue::Type::Array);
        while (m_tokenizer.getToken().type != TokenType::ARRAY_END)
        {
            if (count > 0)
            {
                if (m_tokenizer.getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                m_tokenizer.advance();
            }
            JsonValue newValue;
            if (parseValue(newValue))
            {
                value.append(newValue);
            }
            else
            {
                m_fail = true;
                return false;
            }
            ++count;
        }
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseValue(JsonValue& value)
{
    return parseBoolean(value) ||
           parseNumber(value) ||
           parseString(value) ||
           parseNull(value) ||
           parseArray(value) ||
           parseObject(value);
}

bool Parser::fail() const
{
    return m_fail || m_tokenizer.fail();
}
