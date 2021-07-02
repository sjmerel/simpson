#include "tokenizer.h"
#include <sstream>

int g_level = 0;

bool parseArray(Tokenizer&);
bool parseObject(Tokenizer&);
bool parseElement(Tokenizer&);

void printIndented(const std::string& s)
{
    for (int i = 0; i < g_level; ++i)
    {
        std::cout << "  ";
    }
    std::cout << s << std::endl;
}

bool parseElement(Tokenizer& tokenizer)
{
    /*
    bool result = parseBoolean(tokenizer) ||
                  parseNumber(tokenizer) ||
                  parseString(tokenizer) ||
                  parseNull(tokenizer) ||
                  parseArray(tokenizer) ||
                  parseObject(tokenizer);
                  */
    bool result = true;
    switch (tokenizer.getToken().type)
    {
        case TokenType::BOOLEAN:
            printIndented("boolean:" + tokenizer.getToken().value);
            break;
        case TokenType::NUMBER:
            printIndented("number:" + tokenizer.getToken().value);
            break;
        case TokenType::STRING:
            printIndented("string:" + tokenizer.getToken().value);
            break;
        case TokenType::NULL_:
            printIndented("null");
            break;

        case TokenType::ARRAY_START:
            result = parseArray(tokenizer);
            break;
        case TokenType::ARRAY_END:
            result = parseArray(tokenizer);
            break;

        case TokenType::OBJECT_START:
            result = parseObject(tokenizer);
            break;
        case TokenType::OBJECT_END:
            result = parseObject(tokenizer);
            break;

        default:
            result = false;
            break;
    }
    tokenizer.process();
    return result;
}

bool parseObject(Tokenizer& tokenizer)
{
    return false;
}

bool parseArray(Tokenizer& tokenizer)
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
                return false;
            }
            tokenizer.process();
        }
        if (!parseElement(tokenizer))
        {
            return false;
        }
        ++count;
    }
    --g_level;
    printIndented("]");
    return true;
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << "<input>" << std::endl;
        //std::cerr << std::format("usage: {} <input>\n", argv[0]);
        return 1;
    }

    std::istringstream stream(argv[1]);
    Tokenizer tokenizer(stream);

    tokenizer.process();
    if (!parseElement(tokenizer))
    {
        std::cerr << "parser error" << std::endl;
    }

    /*
    while (tokenizer.process())
    {
        const Token& token = tokenizer.getToken();
        std::cout << (int) token.type << " " << token.value << std::endl;
    }
    */

    if (tokenizer.fail()) 
    { 
        std::cerr << "tokenizer error" << std::endl;
    }

    return 0;
}
