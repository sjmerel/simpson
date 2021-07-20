#pragma once
#include <vector>
#include <string>
#include <map>

class JsonValue
{
public:
    enum class Type
    {
        BOOL,
        NUMBER,
        STRING,
        NULL_,
        ARRAY,
        OBJECT
    };

    JsonValue(); // null

    JsonValue(Type);

    JsonValue(bool);
    JsonValue(double);
    JsonValue(const std::string&);

    // JsonValue(const JsonValue&);
    // JsonValue& operator=(const JsonValue&);
    // TODO move?

    Type type() const { return m_type; }

    bool boolean() const { return m_number; }
    double number() const { return m_number; }
    const std::string& string() const { return m_string; }

    int size() const { return (int) (m_type == Type::ARRAY ? m_array.size() : m_object.size()); }

    // array
    JsonValue& get(int index) { return m_array[index]; }
    const JsonValue& get(int index) const { return m_array[index]; }
    void set(int index, const JsonValue& value);
    void append(const JsonValue& value);

    // object
    JsonValue& get(const std::string& key) { return m_object.find(key)->second; }
    const JsonValue& get(const std::string& key) const { return m_object.find(key)->second; }
    void set(const std::string& key, const JsonValue& value);
    const std::string& getKey(int index) const;

    // void setBool(bool);
    // void setNumber(double);
    // void setSt(double);

private:
    Type m_type;
    std::string m_string;
    double m_number; // or bool
    std::vector<JsonValue> m_array;
    std::map<std::string, JsonValue> m_object;
};
