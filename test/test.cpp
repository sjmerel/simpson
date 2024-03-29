#include "simpson/jsonvalue.h"
#include <fstream>
#include <sstream>

using namespace Simpson;

#define VERIFY(expr) if (!(expr)) \
{ \
    std::cerr << "**** ERROR: line " <<  __LINE__ << ": " << #expr << std::endl; \
    return 1; \
}

int main()
{
    std::cout << "testing..." << std::endl;

    ////////////////////////////////////////

    {
        JsonValue value;

        std::ifstream stream("test.json");
        VERIFY(value.read(stream));

        VERIFY(value[0].string() == value.get(0).string());
        VERIFY(value[0].isString());
        VERIFY(value[0].type() == JsonValue::Type::String);

        VERIFY(value[0].string() == "hello");
        VERIFY(value[1].string() == "hello\"");
        VERIFY(value[2].string() == "hello\\");
        VERIFY(value[3].string() == "\" \\ / / \b \f \n \r \t * ☺ 😀 😀 ☺☺");

        VERIFY(value[4].isNull());

        VERIFY(value[5].isBoolean());
        VERIFY(value[5].boolean());
        VERIFY(!value[6].boolean());

        VERIFY(value[7].isNumber());
        VERIFY(value[7].number() == 0);
        VERIFY(value[8].number() == 0);
        VERIFY(value[9].number() == 1234);
        VERIFY(value[10].number() == 1234);
        VERIFY(value[11].number() == 1234);
        VERIFY(value[12].number() == 1234);
        VERIFY(value[13].number() == -1234);
        VERIFY(value[14].number() == 1234.56);
        VERIFY(value[15].number() == -1e+3);
        VERIFY(value[16].number() == 1e+3);
        VERIFY(value[17].number() == 1.00000300002);

        VERIFY(value[18].boolean());
        VERIFY(!value[19].boolean());

        VERIFY(value[20].isArray());
        VERIFY(value[20].size() == 0);
        VERIFY(value[21].isArray());
        VERIFY(value[21].size() == 0);
        VERIFY(value[22].isArray());
        VERIFY(value[22].size() == 0);

        VERIFY(value[23].isObject());
        VERIFY(value[23].size() == 0);
        VERIFY(value[24].isObject());
        VERIFY(value[24].size() == 0);
        VERIFY(value[25].isObject());
        VERIFY(value[25].size() == 0);

        VERIFY(value[26].isArray());
        VERIFY(value[26].size()==1);
        VERIFY(value[26][0].isArray());
        VERIFY(value[26][0].size() == 1);
        VERIFY(value[27].size() == 6);

        VERIFY(value[28].size() == 1);
        VERIFY(value[28].contains("a"));
        VERIFY(!value[28].contains("g"));
        VERIFY(value[28]["a"].isNumber());
    }

    ////////////////////////////////////////

    {
        // write to file, and read in again; should be equivalent value

        JsonValue value1;

        {
            std::ifstream stream("test.json");
            VERIFY(value1.read(stream));
        }

        {
            std::ofstream stream("test_out.json");
            value1.write(stream);
        }

        JsonValue value2;

        {
            std::ifstream stream("test_out.json");
            VERIFY(value2.read(stream));
        }

        VERIFY(value1 == value2);
    }
   
    ////////////////////////////////////////

    {
        JsonValue a(3.0);
        JsonValue b("hello");
        VERIFY(a != b);
        b = 3;
        VERIFY(a == b);
    }

    ////////////////////////////////////////

    {
        VERIFY(JsonValue(false).isBoolean());
        VERIFY(JsonValue(true).isBoolean());
        VERIFY(JsonValue(0).isNumber());
        VERIFY(JsonValue(0.0).isNumber());
        VERIFY(JsonValue(0.0f).isNumber());
        VERIFY(JsonValue("hello").isString());
        VERIFY(JsonValue(std::string("hello")).isString());
        VERIFY(JsonValue(nullptr).isNull());
        const char* p = nullptr;
        VERIFY(JsonValue(p).isNull());
    }
    
    ////////////////////////////////////////

    {
        JsonValue value(JsonValue::Type::Object);
        VERIFY(value.size() == 0);
        value.set("hello", 2);
        VERIFY(value.size() == 1);
        VERIFY(value.contains("hello"));
        VERIFY(!value.contains("goodbye"));
        value.set("goodbye", 3);
        VERIFY(value.contains("goodbye"));
        value.set("goodbye", nullptr);
        VERIFY(value.contains("goodbye"));
        value.remove("goodbye");
        VERIFY(!value.contains("goodbye"));
        VERIFY(value.size() == 1);
    }

    ////////////////////////////////////////

    {
        JsonValue value(JsonValue::Type::Array);
        VERIFY(value.size() == 0);
        value.append(2);
        VERIFY(value.size() == 1);
        VERIFY(value[0] == 2);
        //value[0] = "hello";
        value.set(0, "hello");
        VERIFY(value[0] == "hello");
        VERIFY(value[0].string() == "hello");
    }

    ////////////////////////////////////////

    {
        JsonValue value;
        VERIFY(value.isInvalid());
        value = false;
        VERIFY(value.isBoolean());
        value = 0;
        VERIFY(value.isNumber());
        value = "hello";
        VERIFY(value.isString());
        value = nullptr;
        VERIFY(value.isNull());
        value = (const char*) 0;
        VERIFY(value.isNull());
    }

    ////////////////////////////////////////

    {
        JsonValue value(JsonValue::Type::Object);
        VERIFY(value.isObject());
        value.set("foo", "bar");
        VERIFY(value["foo"].isString());
        VERIFY(value["hello"].isInvalid());
        VERIFY(value["hello"]["there"].isInvalid());
        VERIFY(value["hello"]["there"][1].isInvalid());
    }

    ////////////////////////////////////////

    {
        std::stringstream str("{\"hello\":\"there\"}");
        JsonValue value;
        VERIFY(value.read(str));
        VERIFY(value["hello"] == "there");
    }

    ////////////////////////////////////////

    std::cout << "...done" << std::endl;
    return 0;
}
