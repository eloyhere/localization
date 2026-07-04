#pragma once

#include <string>
#include <deque>
#include <variant>
#include <sstream>
#include <stdexcept>
#include <cctype>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <iostream>

namespace json
{
enum class Type
{
    Null,
    Number,
    String,
    Boolean,
    Array,
    Object
};

template <Type T>
class Primitive;

template <>
class Primitive<Type::Null>
{
  public:
    std::nullptr_t value;

    Primitive() : value(nullptr) {}
    Primitive(std::nullptr_t) : value(nullptr) {}

    bool operator==(const Primitive &other) const
    {
        return true;
    }

    bool operator!=(const Primitive &other) const
    {
        return false;
    }

    bool operator<(const Primitive &other) const
    {
        return false;
    }

    bool operator<=(const Primitive &other) const
    {
        return true;
    }

    bool operator>(const Primitive &other) const
    {
        return false;
    }

    bool operator>=(const Primitive &other) const
    {
        return true;
    }

    explicit operator bool() const
    {
        return false;
    }

    explicit operator short() const
    {
        return 0;
    }

    explicit operator unsigned short() const
    {
        return 0;
    }

    explicit operator int() const
    {
        return 0;
    }

    explicit operator unsigned int() const
    {
        return 0;
    }

    explicit operator float() const
    {
        return 0.0f;
    }

    explicit operator double() const
    {
        return 0.0;
    }

    explicit operator long double() const
    {
        return 0.0L;
    }

    explicit operator long long() const
    {
        return 0LL;
    }

    explicit operator unsigned long long() const
    {
        return 0ULL;
    }

    explicit operator std::string() const
    {
        return "null";
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        stream << "null";
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        std::string word;
        stream >> word;
        if (word != "null")
        {
            stream.setstate(std::ios::failbit);
        }
        return stream;
    }
};

template <>
class Primitive<Type::Boolean>
{
  public:
    bool value;

    Primitive() : value(false) {}
    Primitive(bool parameter) : value(parameter) {}

    Primitive &operator=(bool parameter)
    {
        value = parameter;
        return *this;
    }

    bool operator==(const Primitive &other) const
    {
        return value == other.value;
    }

    bool operator!=(const Primitive &other) const
    {
        return value != other.value;
    }

    bool operator<(const Primitive &other) const
    {
        return value < other.value;
    }

    bool operator<=(const Primitive &other) const
    {
        return value <= other.value;
    }

    bool operator>(const Primitive &other) const
    {
        return value > other.value;
    }

    bool operator>=(const Primitive &other) const
    {
        return value >= other.value;
    }

    bool operator&(const Primitive &other) const
    {
        return value && other.value;
    }

    bool operator|(const Primitive &other) const
    {
        return value || other.value;
    }

    bool operator^(const Primitive &other) const
    {
        return value != other.value;
    }

    bool operator!() const
    {
        return !value;
    }

    explicit operator bool() const
    {
        return value;
    }

    explicit operator short() const
    {
        return static_cast<short>(value);
    }

    explicit operator unsigned short() const
    {
        return static_cast<unsigned short>(value);
    }

    explicit operator int() const
    {
        return static_cast<int>(value);
    }

    explicit operator unsigned int() const
    {
        return static_cast<unsigned int>(value);
    }

    explicit operator float() const
    {
        return static_cast<float>(value);
    }

    explicit operator double() const
    {
        return static_cast<double>(value);
    }

    explicit operator long double() const
    {
        return static_cast<long double>(value);
    }

    explicit operator long long() const
    {
        return static_cast<long long>(value);
    }

    explicit operator unsigned long long() const
    {
        return static_cast<unsigned long long>(value);
    }

    explicit operator std::string() const
    {
        return value ? "true" : "false";
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        stream << (primitive.value ? "true" : "false");
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        std::string word;
        stream >> word;
        if (word == "true")
        {
            primitive.value = true;
        }
        else if (word == "false")
        {
            primitive.value = false;
        }
        else
        {
            stream.setstate(std::ios::failbit);
        }
        return stream;
    }
};

template <>
class Primitive<Type::Number>
{
  public:
    std::variant<long long, double> value;

    Primitive() : value(0LL) {}
    Primitive(long long parameter) : value(parameter) {}
    Primitive(int parameter) : value(static_cast<long long>(parameter)) {}
    Primitive(unsigned long long parameter) : value(static_cast<long long>(parameter)) {}
    Primitive(double parameter) : value(parameter) {}
    Primitive(float parameter) : value(static_cast<double>(parameter)) {}

    Primitive &operator=(long long parameter)
    {
        value = parameter;
        return *this;
    }

    Primitive &operator=(int parameter)
    {
        value = static_cast<long long>(parameter);
        return *this;
    }

    Primitive &operator=(unsigned long long parameter)
    {
        value = static_cast<long long>(parameter);
        return *this;
    }

    Primitive &operator=(double parameter)
    {
        value = parameter;
        return *this;
    }

    Primitive &operator=(float parameter)
    {
        value = static_cast<double>(parameter);
        return *this;
    }

    bool isInteger() const
    {
        return std::holds_alternative<long long>(value);
    }

    bool isFloating() const
    {
        return std::holds_alternative<double>(value);
    }

    long long asInteger() const
    {
        if (std::holds_alternative<long long>(value))
        {
            return std::get<long long>(value);
        }
        return static_cast<long long>(std::get<double>(value));
    }

    double asFloating() const
    {
        if (std::holds_alternative<double>(value))
        {
            return std::get<double>(value);
        }
        return static_cast<double>(std::get<long long>(value));
    }

    bool operator==(const Primitive &other) const
    {
        return value == other.value;
    }

    bool operator!=(const Primitive &other) const
    {
        return value != other.value;
    }

    bool operator<(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return asInteger() < other.asInteger();
        }
        return asFloating() < other.asFloating();
    }

    bool operator<=(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return asInteger() <= other.asInteger();
        }
        return asFloating() <= other.asFloating();
    }

    bool operator>(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return asInteger() > other.asInteger();
        }
        return asFloating() > other.asFloating();
    }

    bool operator>=(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return asInteger() >= other.asInteger();
        }
        return asFloating() >= other.asFloating();
    }

    Primitive operator+(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return Primitive(asInteger() + other.asInteger());
        }
        return Primitive(asFloating() + other.asFloating());
    }

    Primitive operator-(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return Primitive(asInteger() - other.asInteger());
        }
        return Primitive(asFloating() - other.asFloating());
    }

    Primitive operator*(const Primitive &other) const
    {
        if (isInteger() && other.isInteger())
        {
            return Primitive(asInteger() * other.asInteger());
        }
        return Primitive(asFloating() * other.asFloating());
    }

    Primitive operator/(const Primitive &other) const
    {
        if (isInteger() && other.isInteger() && other.asInteger() != 0)
        {
            return Primitive(asInteger() / other.asInteger());
        }
        return Primitive(asFloating() / other.asFloating());
    }

    Primitive &operator+=(const Primitive &other)
    {
        *this = *this + other;
        return *this;
    }

    Primitive &operator-=(const Primitive &other)
    {
        *this = *this - other;
        return *this;
    }

    Primitive &operator*=(const Primitive &other)
    {
        *this = *this * other;
        return *this;
    }

    Primitive &operator/=(const Primitive &other)
    {
        *this = *this / other;
        return *this;
    }

    explicit operator bool() const
    {
        if (isInteger())
        {
            return asInteger() != 0;
        }
        return asFloating() != 0.0;
    }

    explicit operator short() const
    {
        return static_cast<short>(asInteger());
    }

    explicit operator unsigned short() const
    {
        return static_cast<unsigned short>(asInteger());
    }

    explicit operator int() const
    {
        return static_cast<int>(asInteger());
    }

    explicit operator unsigned int() const
    {
        return static_cast<unsigned int>(asInteger());
    }

    explicit operator float() const
    {
        return static_cast<float>(asFloating());
    }

    explicit operator double() const
    {
        return asFloating();
    }

    explicit operator long double() const
    {
        return static_cast<long double>(asFloating());
    }

    explicit operator long long() const
    {
        return asInteger();
    }

    explicit operator unsigned long long() const
    {
        return static_cast<unsigned long long>(asInteger());
    }

    explicit operator std::string() const
    {
        if (isInteger())
        {
            return std::to_string(asInteger());
        }
        return std::to_string(asFloating());
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        std::visit([&](const auto &number) { stream << number; }, primitive.value);
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        std::string word;
        stream >> word;
        if (word.find('.') != std::string::npos || word.find('e') != std::string::npos || word.find('E') != std::string::npos)
        {
            primitive.value = std::stod(word);
        }
        else
        {
            primitive.value = std::stoll(word);
        }
        return stream;
    }
};

template <>
class Primitive<Type::String>
{
  public:
    std::string value;

    Primitive() = default;
    Primitive(const std::string &parameter) : value(parameter) {}
    Primitive(std::string &&parameter) : value(std::move(parameter)) {}
    Primitive(const char *parameter) : value(parameter) {}

    Primitive &operator=(const std::string &parameter)
    {
        value = parameter;
        return *this;
    }

    Primitive &operator=(std::string &&parameter)
    {
        value = std::move(parameter);
        return *this;
    }

    Primitive &operator=(const char *parameter)
    {
        value = parameter;
        return *this;
    }

    bool operator==(const Primitive &other) const
    {
        return value == other.value;
    }

    bool operator!=(const Primitive &other) const
    {
        return value != other.value;
    }

    bool operator<(const Primitive &other) const
    {
        return value < other.value;
    }

    bool operator<=(const Primitive &other) const
    {
        return value <= other.value;
    }

    bool operator>(const Primitive &other) const
    {
        return value > other.value;
    }

    bool operator>=(const Primitive &other) const
    {
        return value >= other.value;
    }

    Primitive operator+(const Primitive &other) const
    {
        return Primitive(value + other.value);
    }

    Primitive &operator+=(const Primitive &other)
    {
        value += other.value;
        return *this;
    }

    bool empty() const
    {
        return value.empty();
    }

    size_t size() const
    {
        return value.size();
    }

    explicit operator bool() const
    {
        return !value.empty();
    }

    explicit operator short() const
    {
        return static_cast<short>(std::stoll(value));
    }

    explicit operator unsigned short() const
    {
        return static_cast<unsigned short>(std::stoull(value));
    }

    explicit operator int() const
    {
        return static_cast<int>(std::stoll(value));
    }

    explicit operator unsigned int() const
    {
        return static_cast<unsigned int>(std::stoull(value));
    }

    explicit operator float() const
    {
        return std::stof(value);
    }

    explicit operator double() const
    {
        return std::stod(value);
    }

    explicit operator long double() const
    {
        return std::stold(value);
    }

    explicit operator long long() const
    {
        return std::stoll(value);
    }

    explicit operator unsigned long long() const
    {
        return std::stoull(value);
    }

    explicit operator std::string() const
    {
        return value;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        stream << primitive.value;
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        stream >> primitive.value;
        return stream;
    }
};

template <>
class Primitive<Type::Array>
{
  public:
    std::deque<std::string> items;

    Primitive() = default;
    Primitive(const Primitive &) = default;
    Primitive(Primitive &&) noexcept = default;
    Primitive(std::initializer_list<std::string> parameter) : items(parameter) {}

    Primitive &operator=(const Primitive &) = default;
    Primitive &operator=(Primitive &&) noexcept = default;

    std::string &operator[](size_t index)
    {
        return items.at(index);
    }

    const std::string &operator[](size_t index) const
    {
        return items.at(index);
    }

    void pushBack(const std::string &parameter)
    {
        items.push_back(parameter);
    }

    void pushBack(std::string &&parameter)
    {
        items.push_back(std::move(parameter));
    }

    void pushFront(const std::string &parameter)
    {
        items.push_front(parameter);
    }

    void pushFront(std::string &&parameter)
    {
        items.push_front(std::move(parameter));
    }

    void popBack()
    {
        items.pop_back();
    }

    void popFront()
    {
        items.pop_front();
    }

    void insertBack(size_t index, const std::string &parameter)
    {
        if (index >= items.size())
        {
            items.push_back(parameter);
        }
        else
        {
            items.insert(items.begin() + static_cast<long long>(index) + 1, parameter);
        }
    }

    void insertBack(size_t index, std::string &&parameter)
    {
        if (index >= items.size())
        {
            items.push_back(std::move(parameter));
        }
        else
        {
            items.insert(items.begin() + static_cast<long long>(index) + 1, std::move(parameter));
        }
    }

    void removeBack(size_t index)
    {
        if (index < items.size())
        {
            items.erase(items.begin() + static_cast<long long>(index) + 1);
        }
    }

    size_t size() const
    {
        return items.size();
    }

    bool empty() const
    {
        return items.empty();
    }

    auto begin()
    {
        return items.begin();
    }

    auto end()
    {
        return items.end();
    }

    auto begin() const
    {
        return items.begin();
    }

    auto end() const
    {
        return items.end();
    }

    bool operator==(const Primitive &other) const
    {
        return items == other.items;
    }

    bool operator!=(const Primitive &other) const
    {
        return items != other.items;
    }

    bool operator<(const Primitive &other) const
    {
        return items < other.items;
    }

    bool operator<=(const Primitive &other) const
    {
        return items <= other.items;
    }

    bool operator>(const Primitive &other) const
    {
        return items > other.items;
    }

    bool operator>=(const Primitive &other) const
    {
        return items >= other.items;
    }

    explicit operator bool() const
    {
        return !items.empty();
    }

    explicit operator std::string() const
    {
        std::string result = "[";
        for (size_t i = 0; i < items.size(); ++i)
        {
            if (i > 0)
            {
                result += ",";
            }
            result += items[i];
        }
        result += "]";
        return result;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        stream << static_cast<std::string>(primitive);
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        return stream;
    }
};

template <>
class Primitive<Type::Object>
{
  public:
    std::map<std::string, std::string> members;

    Primitive() = default;
    Primitive(const Primitive &) = default;
    Primitive(Primitive &&) noexcept = default;
    Primitive(std::initializer_list<std::pair<const std::string, std::string>> parameter) : members(parameter) {}

    Primitive &operator=(const Primitive &) = default;
    Primitive &operator=(Primitive &&) noexcept = default;

    std::string &operator[](const std::string &key)
    {
        return members[key];
    }

    const std::string &operator[](const std::string &key) const
    {
        return members.at(key);
    }

    bool contains(const std::string &key) const
    {
        return members.count(key) > 0;
    }

    void insert(const std::string &key, const std::string &parameter)
    {
        members.emplace(key, parameter);
    }

    void insert(const std::string &key, std::string &&parameter)
    {
        members.emplace(key, std::move(parameter));
    }

    size_t size() const
    {
        return members.size();
    }

    bool empty() const
    {
        return members.empty();
    }

    auto begin()
    {
        return members.begin();
    }

    auto end()
    {
        return members.end();
    }

    auto begin() const
    {
        return members.begin();
    }

    auto end() const
    {
        return members.end();
    }

    bool operator==(const Primitive &other) const
    {
        return members == other.members;
    }

    bool operator!=(const Primitive &other) const
    {
        return members != other.members;
    }

    bool operator<(const Primitive &other) const
    {
        return members < other.members;
    }

    bool operator<=(const Primitive &other) const
    {
        return members <= other.members;
    }

    bool operator>(const Primitive &other) const
    {
        return members > other.members;
    }

    bool operator>=(const Primitive &other) const
    {
        return members >= other.members;
    }

    explicit operator bool() const
    {
        return !members.empty();
    }

    explicit operator std::string() const
    {
        std::string result = "{";
        bool first = true;
        for (const auto &[key, val] : members)
        {
            if (!first)
            {
                result += ",";
            }
            first = false;
            result += "\"" + key + "\":" + val;
        }
        result += "}";
        return result;
    }

    friend std::ostream &operator<<(std::ostream &stream, const Primitive &primitive)
    {
        stream << static_cast<std::string>(primitive);
        return stream;
    }

    friend std::istream &operator>>(std::istream &stream, Primitive &primitive)
    {
        return stream;
    }
};

enum class Token
{
    Comma,
    Colon,
    Quote,
    ObjectStart,
    ObjectEnd,
    ArrayStart,
    ArrayEnd,
    Value
};

class Sequence
{
  public:
    struct TokenValue
    {
        Token token;
        std::string value;
    };

    std::deque<TokenValue> tokens;

    Sequence() = default;

    Sequence(const std::string &json)
    {
        size_t position = 0;
        while (position < json.size())
        {
            while (position < json.size() && std::isspace(json[position]))
            {
                ++position;
            }
            if (position >= json.size())
            {
                break;
            }
            char character = json[position];
            switch (character)
            {
            case '{':
                push(Token::ObjectStart);
                ++position;
                break;
            case '}':
                push(Token::ObjectEnd);
                ++position;
                break;
            case '[':
                push(Token::ArrayStart);
                ++position;
                break;
            case ']':
                push(Token::ArrayEnd);
                ++position;
                break;
            case ':':
                push(Token::Colon);
                ++position;
                break;
            case ',':
                push(Token::Comma);
                ++position;
                break;
            case '"':
            {
                push(Token::Quote);
                ++position;
                std::string result;
                while (position < json.size() && json[position] != '"')
                {
                    if (json[position] == '\\')
                    {
                        ++position;
                        if (position >= json.size())
                        {
                            throw std::runtime_error("unexpected end of string");
                        }
                        switch (json[position])
                        {
                        case '"':
                            result += '"';
                            break;
                        case '\\':
                            result += '\\';
                            break;
                        case '/':
                            result += '/';
                            break;
                        case 'b':
                            result += '\b';
                            break;
                        case 'f':
                            result += '\f';
                            break;
                        case 'n':
                            result += '\n';
                            break;
                        case 'r':
                            result += '\r';
                            break;
                        case 't':
                            result += '\t';
                            break;
                        default:
                            throw std::runtime_error("invalid escape character");
                        }
                    }
                    else
                    {
                        result += json[position];
                    }
                    ++position;
                }
                push(Token::Value, result);
                push(Token::Quote);
                ++position;
                break;
            }
            default:
            {
                size_t start = position;
                while (position < json.size() &&
                       !std::isspace(json[position]) &&
                       json[position] != ',' &&
                       json[position] != '}' &&
                       json[position] != ']')
                {
                    ++position;
                }
                push(Token::Value, json.substr(start, position - start));
                break;
            }
            }
        }
    }

    void push(Token token, const std::string &value = "")
    {
        tokens.push_back({token, value});
    }

    void push(const TokenValue &tokenValue)
    {
        tokens.push_back(tokenValue);
    }

    TokenValue peek() const
    {
        if (tokens.empty())
        {
            throw std::runtime_error("unexpected end of sequence");
        }
        return tokens.front();
    }

    TokenValue pop()
    {
        if (tokens.empty())
        {
            throw std::runtime_error("unexpected end of sequence");
        }
        auto token = tokens.front();
        tokens.pop_front();
        return token;
    }

    bool empty() const
    {
        return tokens.empty();
    }

    std::string toString() const
    {
        std::string result;
        auto temporarySequence = *this;
        while (!temporarySequence.empty())
        {
            auto token = temporarySequence.pop();
            switch (token.token)
            {
            case Token::ObjectStart:
                result += "{";
                break;
            case Token::ObjectEnd:
                result += "}";
                break;
            case Token::ArrayStart:
                result += "[";
                break;
            case Token::ArrayEnd:
                result += "]";
                break;
            case Token::Quote:
                result += "\"";
                break;
            case Token::Colon:
                result += ":";
                break;
            case Token::Comma:
                result += ",";
                break;
            case Token::Value:
                result += token.value;
                break;
            }
        }
        return result;
    }

    bool isNull() const
    {
        if (tokens.empty())
        {
            return false;
        }
        auto token = tokens.front();
        return token.token == Token::Value && token.value == "null";
    }

    bool isBoolean() const
    {
        if (tokens.empty())
        {
            return false;
        }
        auto token = tokens.front();
        return token.token == Token::Value && (token.value == "true" || token.value == "false");
    }

    bool isNumber() const
    {
        if (tokens.empty())
        {
            return false;
        }
        auto token = tokens.front();
        if (token.token != Token::Value)
        {
            return false;
        }
        if (token.value == "null" || token.value == "true" || token.value == "false")
        {
            return false;
        }
        return true;
    }

    bool isString() const
    {
        if (tokens.empty())
        {
            return false;
        }
        return tokens.front().token == Token::Quote;
    }

    bool isArray() const
    {
        if (tokens.empty())
        {
            return false;
        }
        return tokens.front().token == Token::ArrayStart;
    }

    bool isObject() const
    {
        if (tokens.empty())
        {
            return false;
        }
        return tokens.front().token == Token::ObjectStart;
    }

    template <Type T>
    Primitive<T> as() const
    {
        auto tempSequence = *this;
        if constexpr (T == Type::Null)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::Value || token.value != "null")
            {
                throw std::runtime_error("expected null");
            }
            return Primitive<Type::Null>(nullptr);
        }
        else if constexpr (T == Type::Boolean)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::Value)
            {
                throw std::runtime_error("expected value");
            }
            if (token.value == "true")
            {
                return Primitive<Type::Boolean>(true);
            }
            if (token.value == "false")
            {
                return Primitive<Type::Boolean>(false);
            }
            throw std::runtime_error("invalid boolean: " + token.value);
        }
        else if constexpr (T == Type::Number)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::Value)
            {
                throw std::runtime_error("expected value");
            }
            if (token.value.find('.') != std::string::npos ||
                token.value.find('e') != std::string::npos ||
                token.value.find('E') != std::string::npos)
            {
                return Primitive<Type::Number>(std::stod(token.value));
            }
            else
            {
                return Primitive<Type::Number>(std::stoll(token.value));
            }
        }
        else if constexpr (T == Type::String)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::Quote)
            {
                throw std::runtime_error("expected quote");
            }
            token = tempSequence.pop();
            if (token.token != Token::Value)
            {
                throw std::runtime_error("expected string value");
            }
            std::string result = token.value;
            token = tempSequence.pop();
            if (token.token != Token::Quote)
            {
                throw std::runtime_error("expected closing quote");
            }
            return Primitive<Type::String>(result);
        }
        else if constexpr (T == Type::Array)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::ArrayStart)
            {
                throw std::runtime_error("expected '['");
            }
            Primitive<Type::Array> array;
            if (tempSequence.peek().token == Token::ArrayEnd)
            {
                tempSequence.pop();
                return array;
            }
            while (true)
            {
                Sequence elementSequence;
                auto elementToken = tempSequence.peek();
                if (elementToken.token == Token::ObjectStart)
                {
                    int depth = 1;
                    elementSequence.push(tempSequence.pop());
                    while (depth > 0 && !tempSequence.empty())
                    {
                        auto t = tempSequence.pop();
                        elementSequence.push(t);
                        if (t.token == Token::ObjectStart)
                        {
                            depth++;
                        }
                        else if (t.token == Token::ObjectEnd)
                        {
                            depth--;
                        }
                    }
                }
                else if (elementToken.token == Token::ArrayStart)
                {
                    int depth = 1;
                    elementSequence.push(tempSequence.pop());
                    while (depth > 0 && !tempSequence.empty())
                    {
                        auto t = tempSequence.pop();
                        elementSequence.push(t);
                        if (t.token == Token::ArrayStart)
                        {
                            depth++;
                        }
                        else if (t.token == Token::ArrayEnd)
                        {
                            depth--;
                        }
                    }
                }
                else if (elementToken.token == Token::Quote)
                {
                    elementSequence.push(tempSequence.pop());
                    elementSequence.push(tempSequence.pop());
                    elementSequence.push(tempSequence.pop());
                }
                else if (elementToken.token == Token::Value)
                {
                    elementSequence.push(tempSequence.pop());
                }
                array.items.push_back(elementSequence.toString());
                auto nextToken = tempSequence.pop();
                if (nextToken.token == Token::ArrayEnd)
                {
                    break;
                }
                if (nextToken.token != Token::Comma)
                {
                    throw std::runtime_error("expected ',' or ']'");
                }
            }
            return array;
        }
        else if constexpr (T == Type::Object)
        {
            auto token = tempSequence.pop();
            if (token.token != Token::ObjectStart)
            {
                throw std::runtime_error("expected '{'");
            }
            Primitive<Type::Object> object;
            if (tempSequence.peek().token == Token::ObjectEnd)
            {
                tempSequence.pop();
                return object;
            }
            while (true)
            {
                auto keyTokenStart = tempSequence.pop();
                if (keyTokenStart.token != Token::Quote)
                {
                    throw std::runtime_error("expected '\"'");
                }
                auto keyTokenValue = tempSequence.pop();
                if (keyTokenValue.token != Token::Value)
                {
                    throw std::runtime_error("expected key value");
                }
                std::string key = keyTokenValue.value;
                auto keyTokenEnd = tempSequence.pop();
                if (keyTokenEnd.token != Token::Quote)
                {
                    throw std::runtime_error("expected '\"'");
                }
                token = tempSequence.pop();
                if (token.token != Token::Colon)
                {
                    throw std::runtime_error("expected ':'");
                }
                Sequence valueSequence;
                auto valueToken = tempSequence.peek();
                if (valueToken.token == Token::ObjectStart)
                {
                    int depth = 1;
                    valueSequence.push(tempSequence.pop());
                    while (depth > 0 && !tempSequence.empty())
                    {
                        auto t = tempSequence.pop();
                        valueSequence.push(t);
                        if (t.token == Token::ObjectStart)
                        {
                            depth++;
                        }
                        else if (t.token == Token::ObjectEnd)
                        {
                            depth--;
                        }
                    }
                }
                else if (valueToken.token == Token::ArrayStart)
                {
                    int depth = 1;
                    valueSequence.push(tempSequence.pop());
                    while (depth > 0 && !tempSequence.empty())
                    {
                        auto t = tempSequence.pop();
                        valueSequence.push(t);
                        if (t.token == Token::ArrayStart)
                        {
                            depth++;
                        }
                        else if (t.token == Token::ArrayEnd)
                        {
                            depth--;
                        }
                    }
                }
                else if (valueToken.token == Token::Quote)
                {
                    valueSequence.push(tempSequence.pop());
                    valueSequence.push(tempSequence.pop());
                    valueSequence.push(tempSequence.pop());
                }
                else if (valueToken.token == Token::Value)
                {
                    valueSequence.push(tempSequence.pop());
                }
                object.members.emplace(key, valueSequence.toString());
                auto nextToken = tempSequence.pop();
                if (nextToken.token == Token::ObjectEnd)
                {
                    break;
                }
                if (nextToken.token != Token::Comma)
                {
                    throw std::runtime_error("expected ',' or '}'");
                }
            }
            return object;
        }
    }

    Primitive<Type::Null> asNull() const
    {
        return as<Type::Null>();
    }

    Primitive<Type::Boolean> asBoolean() const
    {
        return as<Type::Boolean>();
    }

    Primitive<Type::Number> asNumber() const
    {
        return as<Type::Number>();
    }

    Primitive<Type::String> asString() const
    {
        return as<Type::String>();
    }

    Primitive<Type::Array> asArray() const
    {
        return as<Type::Array>();
    }

    Primitive<Type::Object> asObject() const
    {
        return as<Type::Object>();
    }
};

template <Type T>
class Encoder
{
  public:
    Sequence operator()(const Primitive<T> &value) const;
};

template <>
class Encoder<Type::Null>
{
  public:
    Sequence operator()(const Primitive<Type::Null> &) const
    {
        Sequence sequence;
        sequence.push(Token::Value, "null");
        return sequence;
    }
};

template <>
class Encoder<Type::Boolean>
{
  public:
    Sequence operator()(const Primitive<Type::Boolean> &value) const
    {
        Sequence sequence;
        sequence.push(Token::Value, value.value ? "true" : "false");
        return sequence;
    }
};

template <>
class Encoder<Type::Number>
{
  public:
    Sequence operator()(const Primitive<Type::Number> &value) const
    {
        Sequence sequence;
        std::ostringstream stream;
        std::visit([&](const auto &number) { stream << number; }, value.value);
        sequence.push(Token::Value, stream.str());
        return sequence;
    }
};

template <>
class Encoder<Type::String>
{
  public:
    Sequence operator()(const Primitive<Type::String> &value) const
    {
        Sequence sequence;
        sequence.push(Token::Quote);
        std::string escaped;
        for (char character : value.value)
        {
            switch (character)
            {
            case '\"':
                escaped += "\\\"";
                break;
            case '\\':
                escaped += "\\\\";
                break;
            case '\b':
                escaped += "\\b";
                break;
            case '\f':
                escaped += "\\f";
                break;
            case '\n':
                escaped += "\\n";
                break;
            case '\r':
                escaped += "\\r";
                break;
            case '\t':
                escaped += "\\t";
                break;
            default:
                escaped += character;
            }
        }
        sequence.push(Token::Value, escaped);
        sequence.push(Token::Quote);
        return sequence;
    }
};

template <>
class Encoder<Type::Array>
{
  public:
    Sequence operator()(const Primitive<Type::Array> &value) const
    {
        Sequence sequence;
        sequence.push(Token::ArrayStart);
        for (size_t i = 0; i < value.items.size(); ++i)
        {
            if (i > 0)
            {
                sequence.push(Token::Comma);
            }
            Sequence elementSequence(value.items[i]);
            while (!elementSequence.empty())
            {
                auto token = elementSequence.pop();
                sequence.push(token);
            }
        }
        sequence.push(Token::ArrayEnd);
        return sequence;
    }
};

template <>
class Encoder<Type::Object>
{
  public:
    Sequence operator()(const Primitive<Type::Object> &value) const
    {
        Sequence sequence;
        sequence.push(Token::ObjectStart);
        bool first = true;
        for (const auto &[key, val] : value.members)
        {
            if (!first)
            {
                sequence.push(Token::Comma);
            }
            first = false;
            sequence.push(Token::Quote);
            sequence.push(Token::Value, key);
            sequence.push(Token::Quote);
            sequence.push(Token::Colon);
            Sequence valueSequence(val);
            while (!valueSequence.empty())
            {
                auto token = valueSequence.pop();
                sequence.push(token);
            }
        }
        sequence.push(Token::ObjectEnd);
        return sequence;
    }
};

template <Type T>
class Decoder
{
  public:
    Primitive<T> operator()(Sequence &sequence) const
    {
        return sequence.as<T>();
    }
};

template <typename T>
struct serialize
{
    std::string operator()(const T &value) const
    {
        if constexpr (std::is_same_v<T, std::nullptr_t>)
        {
            return Encoder<Type::Null>()(Primitive<Type::Null>(nullptr)).toString();
        }
        else if constexpr (std::is_same_v<T, bool>)
        {
            return Encoder<Type::Boolean>()(Primitive<Type::Boolean>(value)).toString();
        }
        else if constexpr (std::is_integral_v<T>)
        {
            return Encoder<Type::Number>()(Primitive<Type::Number>(static_cast<long long>(value))).toString();
        }
        else if constexpr (std::is_floating_point_v<T>)
        {
            return Encoder<Type::Number>()(Primitive<Type::Number>(static_cast<double>(value))).toString();
        }
        else if constexpr (std::is_same_v<T, std::string> || std::is_same_v<T, const char *>)
        {
            return Encoder<Type::String>()(Primitive<Type::String>(value)).toString();
        }
        else
        {
            return "";
        }
    }
};

template <typename T>
struct serialize<std::vector<T>>
{
    std::string operator()(const std::vector<T> &value) const
    {
        Primitive<Type::Array> array;
        for (const auto &item : value)
        {
            serialize<T> serializer;
            array.pushBack(serializer(item));
        }
        return Encoder<Type::Array>()(array).toString();
    }
};

template <typename T>
struct serialize<std::list<T>>
{
    std::string operator()(const std::list<T> &value) const
    {
        Primitive<Type::Array> array;
        for (const auto &item : value)
        {
            serialize<T> serializer;
            array.pushBack(serializer(item));
        }
        return Encoder<Type::Array>()(array).toString();
    }
};

template <typename T>
struct serialize<std::deque<T>>
{
    std::string operator()(const std::deque<T> &value) const
    {
        Primitive<Type::Array> array;
        for (const auto &item : value)
        {
            serialize<T> serializer;
            array.pushBack(serializer(item));
        }
        return Encoder<Type::Array>()(array).toString();
    }
};

template <typename T>
struct serialize<std::set<T>>
{
    std::string operator()(const std::set<T> &value) const
    {
        Primitive<Type::Array> array;
        for (const auto &item : value)
        {
            serialize<T> serializer;
            array.pushBack(serializer(item));
        }
        return Encoder<Type::Array>()(array).toString();
    }
};

template <typename T>
struct serialize<std::unordered_set<T>>
{
    std::string operator()(const std::unordered_set<T> &value) const
    {
        Primitive<Type::Array> array;
        for (const auto &item : value)
        {
            serialize<T> serializer;
            array.pushBack(serializer(item));
        }
        return Encoder<Type::Array>()(array).toString();
    }
};

template <typename K, typename V>
struct serialize<std::map<K, V>>
{
    std::string operator()(const std::map<K, V> &value) const
    {
        Primitive<Type::Object> object;
        for (const auto &[key, val] : value)
        {
            serialize<V> serializer;
            object.insert(key, serializer(val));
        }
        return Encoder<Type::Object>()(object).toString();
    }
};

template <typename K, typename V>
struct serialize<std::unordered_map<K, V>>
{
    std::string operator()(const std::unordered_map<K, V> &value) const
    {
        Primitive<Type::Object> object;
        for (const auto &[key, val] : value)
        {
            serialize<V> serializer;
            object.insert(key, serializer(val));
        }
        return Encoder<Type::Object>()(object).toString();
    }
};

template <typename T>
struct serialize<std::optional<T>>
{
    std::string operator()(const std::optional<T> &value) const
    {
        if (value.has_value())
        {
            serialize<T> serializer;
            return serializer(value.value());
        }
        else
        {
            return Encoder<Type::Null>()(Primitive<Type::Null>(nullptr)).toString();
        }
    }
};

template <typename T>
struct deserialize
{
    T operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Null> decoder;
        decoder(sequence);
        if constexpr (std::is_same_v<T, std::nullptr_t>)
        {
            return nullptr;
        }
        else
        {
            return T{};
        }
    }
};

template <>
struct deserialize<bool>
{
    bool operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Boolean> decoder;
        return decoder(sequence).value;
    }
};

template <>
struct deserialize<int>
{
    int operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Number> decoder;
        return static_cast<int>(decoder(sequence).asInteger());
    }
};

template <>
struct deserialize<long long>
{
    long long operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Number> decoder;
        return decoder(sequence).asInteger();
    }
};

template <>
struct deserialize<unsigned long long>
{
    unsigned long long operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Number> decoder;
        return static_cast<unsigned long long>(decoder(sequence).asInteger());
    }
};

template <>
struct deserialize<float>
{
    float operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Number> decoder;
        return static_cast<float>(decoder(sequence).asFloating());
    }
};

template <>
struct deserialize<double>
{
    double operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Number> decoder;
        return decoder(sequence).asFloating();
    }
};

template <>
struct deserialize<std::string>
{
    std::string operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::String> decoder;
        return decoder(sequence).value;
    }
};

template <>
struct deserialize<std::nullptr_t>
{
    std::nullptr_t operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Null> decoder;
        decoder(sequence);
        return nullptr;
    }
};

template <typename T>
struct deserialize<std::vector<T>>
{
    std::vector<T> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Array> decoder;
        auto array = decoder(sequence);
        std::vector<T> result;
        for (const auto &item : array.items)
        {
            deserialize<T> deserializer;
            result.push_back(deserializer(item));
        }
        return result;
    }
};

template <typename T>
struct deserialize<std::list<T>>
{
    std::list<T> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Array> decoder;
        auto array = decoder(sequence);
        std::list<T> result;
        for (const auto &item : array.items)
        {
            deserialize<T> deserializer;
            result.push_back(deserializer(item));
        }
        return result;
    }
};

template <typename T>
struct deserialize<std::deque<T>>
{
    std::deque<T> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Array> decoder;
        auto array = decoder(sequence);
        std::deque<T> result;
        for (const auto &item : array.items)
        {
            deserialize<T> deserializer;
            result.push_back(deserializer(item));
        }
        return result;
    }
};

template <typename T>
struct deserialize<std::set<T>>
{
    std::set<T> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Array> decoder;
        auto array = decoder(sequence);
        std::set<T> result;
        for (const auto &item : array.items)
        {
            deserialize<T> deserializer;
            result.insert(deserializer(item));
        }
        return result;
    }
};

template <typename T>
struct deserialize<std::unordered_set<T>>
{
    std::unordered_set<T> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Array> decoder;
        auto array = decoder(sequence);
        std::unordered_set<T> result;
        for (const auto &item : array.items)
        {
            deserialize<T> deserializer;
            result.insert(deserializer(item));
        }
        return result;
    }
};

template <typename K, typename V>
struct deserialize<std::map<K, V>>
{
    std::map<K, V> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Object> decoder;
        auto object = decoder(sequence);
        std::map<K, V> result;
        for (const auto &[key, val] : object.members)
        {
            deserialize<V> deserializer;
            result.emplace(key, deserializer(val));
        }
        return result;
    }
};

template <typename K, typename V>
struct deserialize<std::unordered_map<K, V>>
{
    std::unordered_map<K, V> operator()(const std::string &json) const
    {
        Sequence sequence(json);
        Decoder<Type::Object> decoder;
        auto object = decoder(sequence);
        std::unordered_map<K, V> result;
        for (const auto &[key, val] : object.members)
        {
            deserialize<V> deserializer;
            result.emplace(key, deserializer(val));
        }
        return result;
    }
};

template <typename T>
struct deserialize<std::optional<T>>
{
    std::optional<T> operator()(const std::string &json) const
    {
        if (json == "null")
        {
            return std::nullopt;
        }
        deserialize<T> deserializer;
        return deserializer(json);
    }
};

} // namespace json