#include "tokenizer.h"
#include <sstream>
#include <fstream>

int g_level = 0;
bool g_fail = false;

bool parseElement(Tokenizer&);

void printIndented(const std::string& s)
{
    for (int i = 0; i < g_level; ++i)
    {
        std::cout << "  ";
    }
    std::cout << s << std::endl;
}

bool parseBoolean(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::BOOLEAN)
    {
        printIndented(tokenizer.getToken().value);
        tokenizer.process();
        return true;
    }
    else
    {
        return false;
    }
}

bool parseNumber(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::NUMBER)
    {
        printIndented("number: " + tokenizer.getToken().value);
        tokenizer.process();
        return true;
    }
    else
    {
        return false;
    }
}

bool parseString(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::STRING)
    {
        printIndented("\"" + tokenizer.getToken().value + "\"");
        tokenizer.process();
        return true;
    }
    else
    {
        return false;
    }
}

bool parseNull(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::NULL_)
    {
        printIndented("null");
        tokenizer.process();
        return true;
    }
    else
    {
        return false;
    }
}

bool parseObject(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::OBJECT_START)
    {
        int count = 0;
        printIndented("{");
        ++g_level;
        tokenizer.process();
        while (tokenizer.getToken().type != TokenType::OBJECT_END)
        {
            if (count > 0)
            {
                if (tokenizer.getToken().type != TokenType::COMMA)
                {
                    g_fail = true;
                    return false;
                }
                printIndented(",");
                tokenizer.process();
            }
            if (!parseString(tokenizer))
            {
                g_fail = true;
                return false;
            }
            if (tokenizer.getToken().type != TokenType::COLON)
            {
                g_fail = true;
                return false;
            }
            printIndented(":");
            tokenizer.process();
            if (!parseElement(tokenizer))
            {
                g_fail = true;
                return false;
            }
            ++count;
        }
        tokenizer.process();
        --g_level;
        printIndented("}");
        return true;
    }
    else
    {
        return false;
    }
}

bool parseArray(Tokenizer& tokenizer)
{
    if (tokenizer.getToken().type == TokenType::ARRAY_START)
    {
        int count = 0;
        printIndented("[");
        ++g_level;
        tokenizer.process();
        while (tokenizer.getToken().type != TokenType::ARRAY_END)
        {
            if (count > 0)
            {
                if (tokenizer.getToken().type != TokenType::COMMA)
                {
                    g_fail = true;
                    return false;
                }
                printIndented(",");
                tokenizer.process();
            }
            if (!parseElement(tokenizer))
            {
                g_fail = true;
                return false;
            }
            ++count;
        }
        tokenizer.process();
        --g_level;
        printIndented("]");
        return true;
    }
    else
    {
        return false;
    }
}

bool parseElement(Tokenizer& tokenizer)
{
    return parseBoolean(tokenizer) ||
           parseNumber(tokenizer) ||
           parseString(tokenizer) ||
           parseNull(tokenizer) ||
           parseArray(tokenizer) ||
           parseObject(tokenizer);
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << "<input>" << std::endl;
        return 1;
    }

    //std::istringstream stream(argv[1]);
    std::ifstream stream(argv[1]);
    Tokenizer tokenizer(stream);

    tokenizer.process();

    /*
    while (tokenizer.process())
    {
        const Token& token = tokenizer.getToken();
        std::cout << (int) token.type << " " << token.value << std::endl;
    }
    */

    if (!parseElement(tokenizer) || g_fail)
    {
        std::cerr << "parser error" << std::endl;
        return 1;
    }
    if (tokenizer.fail()) 
    { 
        std::cerr << "tokenizer error" << std::endl;
        return 1;
    }

    return 0;
}
