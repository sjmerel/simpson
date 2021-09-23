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

    JsonValue(nullptr_t = nullptr); // null
    JsonValue(Type);
    JsonValue(bool);
    JsonValue(double);
    JsonValue(int);
    JsonValue(const char*); // note that a null pointer here will result in a value of type Null, not String
    JsonValue(const std::string&);

    // value equality
    bool operator==(const JsonValue&) const;
    bool operator!=(const JsonValue&) const;

    // JsonValue(const JsonValue&);
    // JsonValue& operator=(const JsonValue&);
    // TODO move?

    Type type() const { return m_type; }

    // equivalent to comparing return value of type(), but more concise:
    bool isBoolean() const { return m_type == Type::Boolean; }
    bool isNumber() const { return m_type == Type::Number; }
    bool isString() const { return m_type == Type::String; }
    bool isNull() const { return m_type == Type::Null; }
    bool isArray() const { return m_type == Type::Array; }
    bool isObject() const { return m_type == Type::Object; }

    bool boolean() const { return bool(m_number); }
    double number() const { return m_number; }
    const std::string& string() const { return m_string; }

    // array/object
    int size() const { return (int) (m_type == Type::Array ? m_array.size() : m_object.size()); }

    // array
    JsonValue& get(int index) { return m_array[index]; }
    const JsonValue& get(int index) const { return m_array[index]; }
    JsonValue& operator[](int index) { return m_array[index]; }
    const JsonValue& operator[](int index) const { return m_array[index]; }
    void set(int index, const JsonValue& value);
    void append(const JsonValue& value);

    // object
    JsonValue& get(const std::string& key) { return m_object.find(key)->second; }
    const JsonValue& get(const std::string& key) const { return m_object.find(key)->second; }
    JsonValue& operator[](const std::string& key) { return get(key); }
    const JsonValue& operator[](const std::string& key) const { return get(key); }
    void set(const std::string& key, const JsonValue& value);
    bool containsKey(const std::string& key) const;
    const std::string& getKey(int index) const;

    bool readText(std::istream&);
    void writeText(std::ostream&, bool compact = false) const;

private:
    Type m_type;
    std::string m_string;
    double m_number; // or bool
    std::vector<JsonValue> m_array;
    std::map<std::string, JsonValue> m_object;
};


} // namespace Simpson
