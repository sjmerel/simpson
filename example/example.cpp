#include "simpson/reader.h"
#include "simpson/writer.h"
#include "simpson/jsonvalue.h"

#include <fstream>

using namespace Simpson;

// recursively iterate through the JSON data and convert all String values to uppercase
void makeUppercase(JsonValue& value)
{
    if (value.isString())
    {
        std::string s = value.string();
        std::transform(s.begin(), s.end(), s.begin(), ::toupper);
        value = s;
    }
    else if (value.isArray())
    {
        for (int i = 0; i < value.size(); ++i)
        {
            makeUppercase(value[i]);
        }
    }
    else if (value.isObject())
    {
        for (int i = 0; i < value.size(); ++i)
        {
            const std::string& key = value.getKey(i);
            makeUppercase(value[key]);
        }
    }
}

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "usage: " << argv[0] << " <input>" << std::endl;
        return 1;
    }

    std::ifstream stream(argv[1]);

    // We're using the Reader class here, because we want to be able to print the location of errors.
    // Otherwise, we could have just used value.read(stream).
    Reader reader(stream);

    JsonValue value;
    if (!reader.read(value))
    {
        std::cerr << "error at line " << reader.line() << ", column " << reader.column() << std::endl;
        return 1;
    }

    // Find all the string values in the JSON data, and convert them to uppercase.
    // (Not a thing one needs to do normally, but it shows how to iterate through the data and modify it.)
    makeUppercase(value);

    // Again, we're using the Writer class so we can set the indent, but we could also have used value.write(stream).
    Writer writer(std::cout);
    writer.setIndent(4);
    writer.write(value);

    std::cout << std::endl;

    return 0;
}

