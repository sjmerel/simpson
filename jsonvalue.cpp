#include "jsonvalue.h"
#include "parser.h"
#include "writer.h"

JsonValue::JsonValue() :
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

JsonValue::JsonValue(const std::string& value) :
    m_type(Type::String),
    m_string(value),
    m_number(0.0),
    m_array(),
    m_object()
{}

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

const std::string& JsonValue::getKey(int index) const 
{ 
    auto it = m_object.begin();
    std::advance(it, index);
    return it->first; 
}

bool JsonValue::readText(std::istream& stream)
{
    Parser parser(stream);
    return parser.parse(*this);
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
