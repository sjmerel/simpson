#include "parser.h"
#include "writer.h"
#include "jsonvalue.h"

#include <fstream>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input>" << std::endl;
        return 1;
    }

    std::ifstream stream(argv[1]);
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

    Writer writer(std::cout);
    writer.write(value);
    std::cout << std::endl;

    return 0;
}
