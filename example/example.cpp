#include "simpson/reader.h"
#include "simpson/writer.h"
#include "simpson/jsonvalue.h"

#include <fstream>

using namespace Simpson;

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input>" << std::endl;
        return 1;
    }

    std::ifstream stream(argv[1]);
    Reader reader(stream);

    JsonValue value;
    if (!reader.read(value))
    {
        std::cerr << "error at line " << reader.line() << ", column " << reader.column() << std::endl;

        int pos = reader.pos();
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

