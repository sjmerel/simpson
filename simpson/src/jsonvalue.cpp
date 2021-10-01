#include "simpson/jsonvalue.h"
#include "simpson/reader.h"
#include "simpson/writer.h"

namespace Simpson 
{

JsonValue::JsonValue(Type type) :
    m_type(type)
{
    switch (type)
    {
        case Type::Boolean:
            m_data.boolean = false;
            break;

        case Type::Number:
        case Type::Null:
            m_data.number = 0.0;
            break;

        case Type::String:
            m_data.string = new std::string();
            break;

        case Type::Array:
            m_data.array = new std::vector<JsonValue>();
            break;

        case Type::Object:
            m_data.object = new std::map<std::string, JsonValue>();
            break;
    }
}

JsonValue::JsonValue(nullptr_t) :
    m_type(Type::Null)
{
    m_data.number = 0.0;
}

JsonValue::JsonValue(bool value) :
    m_type(Type::Boolean)
{
    m_data.boolean = value;
}

JsonValue::JsonValue(double value) :
    m_type(Type::Number)
{
    m_data.number = value;
}

JsonValue::JsonValue(int value) :
    m_type(Type::Number)
{
    m_data.number = value;
}

JsonValue::JsonValue(const char* value) :
    m_type(value ? Type::String : Type::Null)
{
    if (value)
    {
        m_data.string = new std::string(value);
    }
}

JsonValue::JsonValue(const std::string& value) :
    m_type(Type::String)
{
    m_data.string = new std::string(value);
}

JsonValue::~JsonValue()
{
    switch (m_type)
    {
        case Type::String:
            delete m_data.string;
            break;

        case Type::Array:
            delete m_data.array;
            break;

        case Type::Object:
            delete m_data.object;
            break;
            
        default:
            break;
    }
}

JsonValue::JsonValue(const JsonValue& other):
    m_type(other.m_type)
{
    switch (m_type)
    {
        case Type::Boolean:
        case Type::Number:
        case Type::Null:
            m_data = other.m_data;
            break;

        case Type::String:
            m_data.string = new std::string(*other.m_data.string);
            break;

        case Type::Array:
            m_data.array = new std::vector<JsonValue>(*other.m_data.array);
            break;

        case Type::Object:
            m_data.object = new std::map<std::string, JsonValue>(*other.m_data.object);
            break;
    }
}

JsonValue& JsonValue::operator=(const JsonValue& other)
{
    if (this != &other) 
    { 
        m_type = other.m_type;
        switch (m_type)
        {
            case Type::Boolean:
            case Type::Number:
            case Type::Null:
                m_data = other.m_data;
                break;

            case Type::String:
                delete m_data.string;
                m_data.string = new std::string(*other.m_data.string);
                break;

            case Type::Array:
                delete m_data.array;
                m_data.array = new std::vector<JsonValue>(*other.m_data.array);
                break;

            case Type::Object:
                delete m_data.object;
                m_data.object = new std::map<std::string, JsonValue>(*other.m_data.object);
                break;
        }
    }

    return *this;
}

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
                const std::string& k = key(i);
                if (!other.contains(k)) { return false; }
                if (get(k) != other.get(k)) { return false; }
            }
            return true;
    }

    return false; // we'll never get here, but MSVC warns about it anyway
}

bool JsonValue::operator!=(const JsonValue& other) const
{
    return !(*this == other);
}

bool JsonValue::boolean() const
{
    assertType(Type::Boolean);
    return m_data.boolean;
}

double JsonValue::number() const 
{ 
    assertType(Type::Number);
    return m_data.number; 
}

const std::string& JsonValue::string() const 
{ 
    assertType(Type::String);
    return *m_data.string; 
}

int JsonValue::size() const
{
    if (m_type == Type::Array)
    {
        return (int) m_data.array->size();
    }
    else if (m_type == Type::Object)
    {
        return (int) m_data.object->size();
    }
    else
    {
        throwType();
    }
}

JsonValue& JsonValue::get(int index)
{
    assertType(Type::Array);
    return (*m_data.array)[index];
}

const JsonValue& JsonValue::get(int index) const
{
    assertType(Type::Array);
    return (*m_data.array)[index];
}

JsonValue& JsonValue::operator[](int index)
{
    return get(index);
}

const JsonValue& JsonValue::operator[](int index) const
{
    return get(index);
}

void JsonValue::set(int index, const JsonValue& value)
{
    assertType(Type::Array);
    (*m_data.array)[index] = value;
}

void JsonValue::remove(int index)
{
    assertType(Type::Array);
    m_data.array->erase(m_data.array->begin() + index);
}

void JsonValue::reserve(int size)
{
    assertType(Type::Array);
    m_data.array->reserve(size);
}

void JsonValue::append(const JsonValue& value)
{
    assertType(Type::Array);
    m_data.array->push_back(value);
}

const JsonValue& JsonValue::get(const std::string& key) const
{
    assertType(Type::Object);
    return m_data.object->find(key)->second;
}

const JsonValue& JsonValue::operator[](const std::string& key) const
{
    return get(key);
}

void JsonValue::set(const std::string& key, const JsonValue& value)
{
    assertType(Type::Object);
    m_data.object->insert(std::make_pair(key, value));
}

void JsonValue::remove(const std::string& key)
{
    m_data.object->erase(key);
}

bool JsonValue::contains(const std::string& key) const
{
    assertType(Type::Object);
    return m_data.object->find(key) != m_data.object->end();
}

const std::string& JsonValue::key(int index) const 
{ 
    assertType(Type::Object);
    auto it = m_data.object->begin();
    std::advance(it, index);
    return it->first; 
}

bool JsonValue::read(std::istream& stream)
{
    Reader reader(stream);
    return reader.read(*this);
}

void JsonValue::write(std::ostream& stream, bool compact) const
{
    Writer writer(stream);
    if (compact)
    {
        writer.setIndent(0);
    }
    writer.write(*this);
}

////////////////////////////////////////

void JsonValue::throwType() const
{
    throw std::runtime_error("incorrect JSON type"); 
}

void JsonValue::assertType(JsonValue::Type type) const
{
    if (m_type != type) 
    { 
        throwType();
    }
}

} // namespace Simpson
