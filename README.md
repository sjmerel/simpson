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
See `example/example.cpp`.
