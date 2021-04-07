#pragma once

#include <string>

namespace lua
{
    enum TokenType
    {
        TOKEN_NULL,
        TOKEN_STRING,
        TOKEN_NUMBER,
        TOKEN_IDENTIFIER,
        TOKEN_LPAREN,
        TOKEN_RPAREN,
        TOKEN_COMMENT,
        TOKEN_OPERATOR
    };

    class Token
    {
    public:
        virtual ~Token() = 0;
        virtual TokenType Type() const = 0;
        virtual std::string ToString() const = 0;
        virtual uint64_t ToNumber() const = 0;
    };

    class NullToken : public Token
    {
    public:
        NullToken(){};
        TokenType Type() const override { return TOKEN_NULL; }
        std::string ToString() const { return "<NULL_TOKEN>"; }
        uint64_t ToNumber() const { return 0; }
    };

    class NumberToken : public Token
    {
    public:
        NumberToken(const uint64_t value) : m_value(value){};
        TokenType Type() const override { return TOKEN_NUMBER; }
        std::string ToString() const { return std::to_string(m_value); }
        uint64_t ToNumber() const { return m_value; }

    private:
        const uint64_t m_value;
    };

    class StringToken : public Token
    {
    public:
        StringToken(const std::string value) : m_value(value){};
        TokenType Type() const override { return TOKEN_STRING; }
        std::string ToString() const { return m_value; }
        uint64_t ToNumber() const { return 0; }

    private:
        const std::string m_value;
    };

    class IdentifierToken : public Token
    {
    public:
        IdentifierToken(std::string value) : m_value(value){};
        TokenType Type() const override { return TOKEN_IDENTIFIER; }
        std::string ToString() const { return m_value; }
        uint64_t ToNumber() const { return 0; }

    private:
        const std::string m_value;
    };

    class LParenToken : public Token
    {
    public:
        TokenType Type() const override { return TOKEN_LPAREN; }
        std::string ToString() const { return "("; }
        uint64_t ToNumber() const { return 0; }
    };

    class RParenToken : public Token
    {
    public:
        TokenType Type() const override { return TOKEN_RPAREN; }
        std::string ToString() const { return ")"; }
        uint64_t ToNumber() const { return 0; }
    };

    class CommentToken : public Token
    {
    public:
        CommentToken(std::string value) : m_value(value){};
        TokenType Type() const override { return TOKEN_COMMENT; }
        std::string ToString() const { return m_value; }
        uint64_t ToNumber() const { return 0; }

    private:
        const std::string m_value;
    };

    class OperatorToken : public Token
    {
    public:
        OperatorToken(std::string value) : m_value(value){};

        TokenType Type() const override { return TOKEN_OPERATOR; }
        std::string ToString() const { return m_value; }
        uint64_t ToNumber() const { return 0; }

    private:
        const std::string m_value;
    };
}