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
        Object
    };

    JsonValue(Type);
    JsonValue(nullptr_t = nullptr); // null
    JsonValue(bool);
    JsonValue(double);
    JsonValue(int);
    JsonValue(const char*); // note that a null pointer here will result in a value of type Null, not String
    JsonValue(const std::string&);

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

    bool boolean() const;
    double number() const;
    const std::string& string() const;

    // array/object
    int size() const;

    // array
    JsonValue& get(int index);
    const JsonValue& get(int index) const;
    JsonValue& operator[](int index);
    const JsonValue& operator[](int index) const;
    void set(int index, const JsonValue& value);
    void reserve(int size);
    void append(const JsonValue& value);

    // object
    JsonValue& get(const std::string& key);
    const JsonValue& get(const std::string& key) const;
    JsonValue& operator[](const std::string& key);
    const JsonValue& operator[](const std::string& key) const;
    void set(const std::string& key, const JsonValue& value);
    bool containsKey(const std::string& key) const;
    const std::string& getKey(int index) const;

    // serialization
    bool readText(std::istream&);
    void writeText(std::ostream&, bool compact = false) const;

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
};


} // namespace Simpson
