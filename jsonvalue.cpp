#include "jsonvalue.h"

JsonValue::JsonValue() :
    m_type(Type::NULL_),
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
    m_type(Type::BOOL),
    m_string(),
    m_number(value),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(double value) :
    m_type(Type::NUMBER),
    m_string(),
    m_number(value),
    m_array(),
    m_object()
{}

JsonValue::JsonValue(const std::string& value) :
    m_type(Type::STRING),
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
