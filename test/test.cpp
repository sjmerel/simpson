#include "simpson/jsonvalue.h"
#include <fstream>

using namespace Simpson;

#define VERIFY(expr) if (!(expr)) \
{ \
    std::cerr << "**** ERROR: line " <<  __LINE__ << std::endl; \
    std::cerr << #expr << std::endl; \
    return 1; \
}

int main()
{
    std::ifstream stream("test.json");

    JsonValue value;
    VERIFY( value.readText(stream) );

    VERIFY(value[0].string() == value.get(0).string());
    VERIFY(value[0].isString());
    VERIFY(value[0].type() == JsonValue::Type::String);

    VERIFY(value[0].string() == "hello");
    VERIFY(value[1].string() == "hello\"");
    VERIFY(value[2].string() == "hello\\");
    VERIFY(value[3].string() == "hello\nthere\bone\ttwo");

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

    return 0;
}
