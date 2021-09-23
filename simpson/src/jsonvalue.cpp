#include "simpson/jsonvalue.h"
#include "simpson/reader.h"
#include "simpson/writer.h"

namespace Simpson 
{

JsonValue::JsonValue(nullptr_t) :
    m_type(Type::Null),
    m_string(),
    m_number(0.0),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(Type type) :
    m_type(type),
    m_string(),
    m_number(0.0),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(bool value) :
    m_type(Type::Boolean),
    m_string(),
    m_number(value),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(double value) :
    m_type(Type::Number),
    m_string(),
    m_number(value),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(int value) :
    m_type(Type::Number),
    m_string(),
    m_number(value),
    m_array(),
    m_object()
{}

// this will also be called for any null pointer.
JsonValue::JsonValue(const char* value) :
    m_type(value ? Type::String : Type::Null),
    m_string(),
    m_number(0.0),
    m_array(),
    m_object()
{
    if (value)
    {
        m_string = value;
    }
}

JsonValue::JsonValue(const std::string& value) :
    m_type(Type::String),
    m_string(value),
    m_number(0.0),
    m_array(),
    m_object()
{}

bool JsonValue::operator==(const JsonValue& other) const
{
    if (m_type != other.m_type) { return false; }

    switch (m_type)
    {
        case Type::Boolean:
            return boolean() == other.boolean();

        case Type::Number:
            return number() == other.number();

        case Type::String:
            return string() == other.string();

        case Type::Null:
            return true;

        case Type::Array:
            if (size() != other.size()) { return false; }
            for (int i = 0; i < size(); ++i)
            {
                if (get(i) != other.get(i)) { return false; }
            }
            return true;

        case Type::Object:
            if (size() != other.size()) { return false; }
            for (int i = 0; i < size(); ++i)
            {
                const std::string& key = getKey(i);
                if (!other.containsKey(key)) { return false; }
                if (get(key) != other.get(key)) { return false; }
            }
            return true;
    }
}

bool JsonValue::operator!=(const JsonValue& other) const
{
    return !(*this == other);
}

void JsonValue::set(int index, const JsonValue& value)
{
    m_array[index] = value;
}

void JsonValue::append(const JsonValue& value)
{
    m_array.push_back(value);
}

void JsonValue::set(const std::string& key, const JsonValue& value)
{
    m_object.insert(std::make_pair(key, value));
}

bool JsonValue::containsKey(const std::string& key) const
{
    return m_object.find(key) != m_object.end();
}

const std::string& JsonValue::getKey(int index) const 
{ 
    auto it = m_object.begin();
    std::advance(it, index);
    return it->first; 
}

bool JsonValue::readText(std::istream& stream)
{
    Reader reader(stream);
    return reader.read(*this);
}

void JsonValue::writeText(std::ostream& stream, bool compact) const
{
    Writer writer(stream);
    if (compact)
    {
        writer.setIndent(0);
    }
    writer.write(*this);
}

} // namespace Simpson
