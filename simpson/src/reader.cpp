#include "simpson/reader.h"
#include "simpson/src/tokenizer.h"
#include <iostream>
#include <sstream>

namespace Simpson 
{

Reader::Reader(std::istream& stream) :
    m_tokenizer(new Tokenizer(stream))
{}

Reader::~Reader()
{
    delete m_tokenizer;
}

bool Reader::read(JsonValue& value)
{
    value.clear();
    m_tokenizer->advance();
    return parseValue(value);
}

int Reader::line() const
{
    return m_tokenizer->line();
}

int Reader::column() const
{
    return m_tokenizer->column();
}

int Reader::pos() const
{
    return m_tokenizer->pos();
}

////////////////////////////////////////

bool Reader::parseBoolean(JsonValue& value)
{
    if (m_tokenizer->getToken().type == TokenType::BOOLEAN)
    {
        value = m_tokenizer->getToken().value == "true" ? true : false;
        m_tokenizer->advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Reader::parseNumber(JsonValue& value)
{
    if (!fail() && m_tokenizer->getToken().type == TokenType::NUMBER)
    {
        // TODO optionally handle NaN/Infinity/-Infinity?
        //  note that C printf standard is nan/inf/-inf, or NAN/INF/-INF, while the Java standard is 
        //  NaN/Infinity/-Infinity, and other languages are probably different.
        std::istringstream stream(m_tokenizer->getToken().value);
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
            m_tokenizer->advance();
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Reader::parseString(JsonValue& value)
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

bool Reader::parseString(std::string& value)
{
    if (!fail() && m_tokenizer->getToken().type == TokenType::STRING)
    {
        value = m_tokenizer->getToken().value;
        m_tokenizer->advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Reader::parseNull(JsonValue& value)
{
    if (!fail() && m_tokenizer->getToken().type == TokenType::NULL_)
    {
        value = JsonValue();
        m_tokenizer->advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Reader::parseObject(JsonValue& value)
{
    if (!fail() && m_tokenizer->getToken().type == TokenType::OBJECT_START)
    {
        int count = 0;
        m_tokenizer->advance();
        value = JsonValue(JsonValue::Type::Object);
        while (!fail() && m_tokenizer->getToken().type != TokenType::OBJECT_END)
        {
            if (count > 0)
            {
                if (m_tokenizer->getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                m_tokenizer->advance();
            }

            std::string key;
            JsonValue newValue;

            if (!parseString(key))
            {
                m_fail = true;
                return false;
            }
            if (m_tokenizer->getToken().type != TokenType::COLON)
            {
                m_fail = true;
                return false;
            }
            m_tokenizer->advance();
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
        m_tokenizer->advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Reader::parseArray(JsonValue& value)
{
    if (!fail() && m_tokenizer->getToken().type == TokenType::ARRAY_START)
    {
        int count = 0;
        m_tokenizer->advance();
        value = JsonValue(JsonValue::Type::Array);
        while (m_tokenizer->getToken().type != TokenType::ARRAY_END)
        {
            if (count > 0)
            {
                if (m_tokenizer->getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                m_tokenizer->advance();
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
        m_tokenizer->advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Reader::parseValue(JsonValue& value)
{
    return parseBoolean(value) ||
           parseNumber(value) ||
           parseString(value) ||
           parseNull(value) ||
           parseArray(value) ||
           parseObject(value);
}

bool Reader::fail() const
{
    return m_fail || m_tokenizer->fail();
}

} // namespace Simpson
