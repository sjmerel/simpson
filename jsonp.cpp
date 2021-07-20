#include "parser.h"
#include <fstream>

#include "tokenizer.h"
#include "jsonvalue.h"
// #include <sstream>

void printIndent(int indent)
{
    for (int i = 0; i < indent; ++i) 
    { 
        std::cerr << " "; 
    }
}

std::string escape(const std::string& s)
{
    std::string result = s;
    for (int i = 0; i < s.length(); ++i)
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
            // case '/':
            default: 
                break;
                // TODO unicode
        }
        if (!repl.empty())
        {
            result.replace(i++, 1, repl);
        }
    }
    return result;
}

void printJson(const JsonValue& value, int indent = 0, const std::string& prefix = "")
{
    printIndent(indent);
    std::cerr << prefix;
    switch (value.type())
    {
        case JsonValue::Type::BOOL:
            std::cerr << (value.boolean() ? "true" : "false");
            break;

        case JsonValue::Type::NUMBER:
            std::cerr << value.number();
            break;

        case JsonValue::Type::STRING:
            std::cerr << "\"" << escape(value.string()) << "\"";
            break;

        case JsonValue::Type::NULL_:
            std::cerr << "null";
            break;

        case JsonValue::Type::ARRAY:
            if (value.size() == 0)
            {
                std::cerr << "[]" << std::endl;
            }
            else
            {
                std::cerr << "[" << std::endl;
                for (int i = 0; i < value.size(); ++i)
                {
                    printJson(value.get(i), indent+2);
                    std::cerr << "," << std::endl;;
                }
                printIndent(indent);
                std::cerr << "]";
            }
            break;

        case JsonValue::Type::OBJECT:
            std::cerr << "{" << std::endl;
            for (int i = 0; i < value.size(); ++i)
            {
                const std::string& key = value.getKey(i);
                const JsonValue& keyValue = value.get(key);
                std::string prefix = "\"" + key + "\": ";
                printJson(keyValue, indent+2, prefix);
                std::cerr << "," << std::endl;
            }
            printIndent(indent);
            std::cerr << "}";
            break;
    }
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
#if 1
    Parser parser(stream);

    JsonValue value;
    if (!parser.parse(value))
    {
        std::cerr << "error at line " << parser.line() << ", column " << parser.column() << std::endl;

        int pos = parser.pos();
        stream.seekg(pos - 10);
        while (stream.tellg() < pos)
        {
            std::cerr << (char)stream.get();
        }
        std::cerr << "<--" << std::endl;

        return 1;
    }
#else
    Tokenizer tokenizer(stream);
    while (tokenizer.advance())
    {
        const Token& token = tokenizer.getToken();
        std::cout << (int) token.type << " " << token.value << std::endl;
    }
    if (tokenizer.fail())
    {
        std::cerr << "error at line " << tokenizer.line() << ", column " << tokenizer.column() << std::endl;
    }
#endif

    printJson(value);

    return 0;
}
