#include "parser.h"
#include <iostream>
#include <sstream>

Parser::Parser(std::istream& stream) :
    m_tokenizer(stream)
{}

bool Parser::parse()
{
    m_tokenizer.advance();
    return parseValue();
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

void Parser::printIndented(const std::string& s)
{
    for (int i = 0; i < m_level; ++i)
    {
        std::cout << "  ";
    }
    std::cout << s << std::endl;
}

bool Parser::parseBoolean()
{
    if (m_tokenizer.getToken().type == TokenType::BOOLEAN)
    {
        printIndented(m_tokenizer.getToken().value);
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseNumber()
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
            printIndented("number: " + m_tokenizer.getToken().value + " (" + std::to_string(d) + ")");
            m_tokenizer.advance();
            return true;
        }
    }
    else
    {
        return false;
    }
}

bool Parser::parseString()
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::STRING)
    {
        printIndented("\"" + m_tokenizer.getToken().value + "\"");
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseNull()
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::NULL_)
    {
        printIndented("null");
        m_tokenizer.advance();
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseObject()
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::OBJECT_START)
    {
        int count = 0;
        printIndented("{");
        ++m_level;
        m_tokenizer.advance();
        while (!fail() && m_tokenizer.getToken().type != TokenType::OBJECT_END)
        {
            if (count > 0)
            {
                if (m_tokenizer.getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                printIndented(",");
                m_tokenizer.advance();
            }
            if (!parseString())
            {
                m_fail = true;
                return false;
            }
            if (m_tokenizer.getToken().type != TokenType::COLON)
            {
                m_fail = true;
                return false;
            }
            printIndented(":");
            m_tokenizer.advance();
            if (!parseValue())
            {
                m_fail = true;
                return false;
            }
            ++count;
        }
        m_tokenizer.advance();
        --m_level;
        printIndented("}");
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseArray()
{
    if (!fail() && m_tokenizer.getToken().type == TokenType::ARRAY_START)
    {
        int count = 0;
        printIndented("[");
        ++m_level;
        m_tokenizer.advance();
        while (m_tokenizer.getToken().type != TokenType::ARRAY_END)
        {
            if (count > 0)
            {
                if (m_tokenizer.getToken().type != TokenType::COMMA)
                {
                    m_fail = true;
                    return false;
                }
                printIndented(",");
                m_tokenizer.advance();
            }
            if (!parseValue())
            {
                m_fail = true;
                return false;
            }
            ++count;
        }
        m_tokenizer.advance();
        --m_level;
        printIndented("]");
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::parseValue()
{
    return parseBoolean() ||
           parseNumber() ||
           parseString() ||
           parseNull() ||
           parseArray() ||
           parseObject();
}

bool Parser::fail() const
{
    return m_fail || m_tokenizer.fail();
}
