# simpson
A simple C++ JSON parser

This is a C++ API for reading, writing, and manipulating JSON data.

Design goals:
1. Simple, easy to understand API
1. Simple implementation (or at least, no more complex than it needs to be)
1. Reasonable performance
1. Portability
1. Conformance to the JSON specification

It is written using standard C++ (at least the C++11 variant), so should compile on any platform that supports C++11.

## Building

1. Add the C++ files in simpson/src to your project.
1. Add the top-level directory to your header search path
1. In your code, include the header files like this:
`#include "simpson/jsonvalue.h"`

Project files are included for the test and example programs for Windows (Visual Studio 2019) and macOS (Xcode).

## Usage

Create JSON data in code:

```
JsonValue value(JsonValue::Type::Object); // create an object
value.set("a", 3.2);
value.set("b", "hello");

// value is now: { "a": 3.2, "b": "hello" }
```

Write it to a file:
```
std::ofstream stream("file.json");
value.write(stream);
```

Write to stdout:
```
value.write(std::cout);
```

To read a JSON file:
```
std::ifstream stream("file.json");
value.read(stream);
```

Parse a string:
```
std::stringstream stream("{\"hello\": \"there\"}");
value.read(stream);
```

Query values:
```
double a = value["a"].number();
std::string b = value["b"].string();
```

See also [example/example.cpp](example/example.cpp).

## Subscript operator vs. get()

You can access JSON object values with the get() function or the array subscript operator (i.e. operator[]). Note that the behavior is different; the get() function will throw an exception if the key is not found in the object, whereas the subscript operator will return a value of type Invalid.
```
// assume obj is { "hello": "there" }
obj.get("goodbye"); // this will throw an exception
obj["goodbye"]; // this will return an Invalid JsonValue
```

Invalid values do not correspond to any actual JSON types, but are a sort of dummy object. Querying an Invalid value for any key will return another Invalid value. This lets you do this kind of convenient chaining of subscript calls:
```
JsonValue value = obj["user"]["date_of_birth"]["year"];
if (value.isNumber())
{
   int year = value.numberInt();
}
```
If any of those keys don't exist, the value is invalid, and `isNumber()` will return false.

Or, similarly:
```
int year = obj["user"]["date_of_birth"]["year"].number(-1); // will return -1 if invalid
```

Array values behave similarly; if you access an out-of-range array element with the subscript operator, it will return an Invalid value.

## License

MIT license; see [LICENSE.txt](LICENSE.txt).
