#include "parser.h"
#include <fstream>

#include "tokenizer.h"
// #include <sstream>


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

    if (!parser.parse())
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


    return 0;
}
