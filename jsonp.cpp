#include "parser.h"
// #include <sstream>
#include <fstream>


int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << "<input>" << std::endl;
        return 1;
    }

    //std::istringstream stream(argv[1]);
    std::ifstream stream(argv[1]);
    Parser parser(stream);

    // parser.process();

    /*
    while (tokenizer.process())
    {
        const Token& token = tokenizer.getToken();
        std::cout << (int) token.type << " " << token.value << std::endl;
    }
    */

    if (!parser.parse())
    {
        std::cerr << "error" << std::endl;
        return 1;
    }

    return 0;
}
