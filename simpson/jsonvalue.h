#pragma once

#include <vector>
#include <string>
#include <map>
#include <iostream>

namespace Simpson 
{


class JsonValue
{
public:
    enum class Type
    {
        Boolean,
        Number,
        String,
        Null,
        Array,
        Object,

        // Invalid is not a true JSON type, but a dummy type that lets you chain object/array subscripts.
        Invalid
    };

    // default-constructed value of specified type
    JsonValue(Type = Type::Invalid);

    JsonValue(nullptr_t); // Null type
    JsonValue(bool); // Boolean type
    JsonValue(double); // Number type
    JsonValue(int); // Number type
    JsonValue(const std::string&); // String type
    JsonValue(const char*); // String type (unless pointer is null, in which case type is then Null)

    ~JsonValue();
    JsonValue(const JsonValue&);
    JsonValue& operator=(const JsonValue&);

    // value equality
    bool operator==(const JsonValue&) const;
    bool operator!=(const JsonValue&) const;

    Type type() const { return m_type; }

    // equivalent to comparing return value of type(), but more concise:
    bool isBoolean() const { return m_type == Type::Boolean; }
    bool isNumber() const { return m_type == Type::Number; }
    bool isString() const { return m_type == Type::String; }
    bool isNull() const { return m_type == Type::Null; }
    bool isArray() const { return m_type == Type::Array; }
    bool isObject() const { return m_type == Type::Object; }

    bool isInvalid() const { return m_type == Type::Invalid; }
    bool isValid() const { return m_type != Type::Invalid; }
    explicit operator bool() const { return m_type != Type::Invalid; }

    // get primitive values
    bool boolean() const;
    double number() const;
    float numberFloat() const { return (float) number(); }
    int numberInt() const { return (int) number(); }
    const std::string& string() const;

    // get primitive values, or default if invalid
    bool boolean(bool def) const;
    double number(double def) const;
    float numberFloat(float def) const;
    int numberInt(int def) const;
    const std::string& string(const std::string def) const;

    // array/object
    int size() const;
    void clear();

    // array
    JsonValue& get(int index);
    const JsonValue& get(int index) const;
    JsonValue& operator[](int index);
    const JsonValue& operator[](int index) const;
    void set(int index, const JsonValue& value);
    void remove(int index);
    void reserve(int size);
    void append(const JsonValue& value);

    // object
    JsonValue& get(const std::string& key);
    const JsonValue& get(const std::string& key) const;
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    void set(const std::string& key, const JsonValue& value);
    void remove(const std::string& key);
    bool contains(const std::string& key) const;
    const std::string& key(int index) const;

    // serialization
    bool read(std::istream&);
    void write(std::ostream&, bool compact = false) const;

private:
    Type m_type;

    union Data
    {
        bool boolean;
        double number;
        std::string* string;
        std::vector<JsonValue>* array;
        std::map<std::string, JsonValue>* object;
    };

    Data m_data;

    void assertType(Type) const;
    [[ noreturn ]] void throwTypeError() const;
};


} // namespace Simpson
