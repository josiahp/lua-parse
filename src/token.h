#pragma once

#include <string>

namespace lua
{
    enum TokenType {
        TOKEN_NULL,
        TOKEN_STRING,
        TOKEN_NUMBER
    };

    class Token
    {
    public:
        virtual ~Token() = 0;
        virtual TokenType Type() = 0;
        virtual std::string ToString() = 0;
        virtual uint64_t ToNumber() = 0;
    };

    class NullToken : public Token
    {
    public:
        NullToken() {};
        TokenType Type() override { return TOKEN_NULL; }
        std::string ToString() { return "<NULL_TOKEN>"; }
        uint64_t ToNumber() { return 0; }
    };

    class NumberToken : public Token
    {
    public:
        NumberToken(const uint64_t value) : m_value(value){};
        TokenType Type() override { return TOKEN_STRING; }
        std::string ToString() { return std::to_string(m_value); }
        uint64_t ToNumber() { return m_value; }

    private:
        const uint64_t m_value;
    };

    class StringToken : public Token
    {
    public:
        StringToken(const std::string value) : m_value(value){};
        TokenType Type() override { return TOKEN_STRING; }
        std::string ToString() { return m_value; }
        uint64_t ToNumber() { return 0; }

    private:
        const std::string m_value;
    };

    /*

    class NumberToken : public Token {

    };*/
}