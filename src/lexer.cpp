#include <iostream>

#include "lexer.h"

namespace lua
{

    Lexer::Lexer(std::iostream &input) : m_input(input)
    {
        if (m_input.good())
            m_input >> std::ws;

        m_currentToken = new NullToken();
        m_nextToken = ParseNextToken();
    }

    bool Lexer::HasTokens() const
    {
        return m_input.good() || m_nextToken->Type() != TOKEN_NULL;
    }

    Token* Lexer::PeekNextToken() const {
        return m_nextToken;
    }

    Token *Lexer::GetToken()
    {
        delete m_currentToken;
        m_currentToken = m_nextToken;
        m_nextToken = ParseNextToken();
        return m_currentToken;
    }

    Token *Lexer::ParseNextToken() {
        Token *tok = nullptr;
        unsigned char c;

        m_input >> std::ws;
        m_input >> std::noskipws;

        // Check for early EOF
        if (!m_input.good())
            return new NullToken();

        c = m_input.peek();

        if (std::isalpha(c))
        {
            tok = ReadIdentifierToken(c);
        }
        else if (std::isdigit(c))
        {
            tok = ReadNumberToken();
        }
        else if (std::ispunct(c))
        {
            switch (c)
            {
            case '"':
                tok = ReadStringToken(c);
                break;
            case '(':
                tok = ReadLParenToken();
                break;
            case ')':
                tok = ReadRParenToken();
                break;
            case '-':
            case '=':
            case '*':
            case '.':
                unsigned char op = m_input.get();
                tok = ReadMulticharacterToken(op);
                break;
            }
        }

        if (tok == nullptr)
            tok = new NullToken();

        m_input >> std::ws;

        return tok;
    }

    StringToken *Lexer::ReadStringToken(unsigned char c)
    {
        std::string buffer;

        // Strip off the leading double quote
        m_input.get();
        c = m_input.peek();

        while (m_input.good() && c != '"')
        {
            m_input >> c;
            buffer += c;
            c = m_input.peek();
        }

        // Strip off the trailing double quote
        if (m_input.good() && c == '"')
            m_input.get();

        return new StringToken(buffer);
    }

    IdentifierToken *Lexer::ReadIdentifierToken(unsigned char c)
    {
        std::string buffer;
        while (m_input.good() && std::isalnum(c))
        {
            m_input >> c;
            buffer += c;
            c = m_input.peek();
        }

        return new IdentifierToken(buffer);
    }

    NumberToken *Lexer::ReadNumberToken()
    {
        uint64_t value;
        m_input >> value;
        return new NumberToken(value);
    }

    LParenToken *Lexer::ReadLParenToken()
    {
        // We already know it's a left paren so chomp off the known character.
        m_input.get();
        return new LParenToken();
    }

    RParenToken *Lexer::ReadRParenToken()
    {
        // We already know it's a right paren so chomp off the known character.
        m_input.get();
        return new RParenToken();
    }

    CommentToken *Lexer::ReadCommentToken()
    {
        std::string buffer;
        m_input >> std::ws;
        std::getline(m_input, buffer);
        return new CommentToken(buffer);
    }

    Token *Lexer::ReadMulticharacterToken(unsigned char op)
    {
        // Prepare lookahead for two-character operators.
        unsigned char look = m_input.peek();
        Token *t = nullptr;

        switch (op)
        {
        case '-':
            if (look == '-')
            {
                // We've only peeked, so consume the next character.
                m_input >> look;
                t = ReadCommentToken();
                break;
            }
            return new OperatorToken("-");
            break;
        case '=':
            if (look == '=')
            {
                // We've only peeked, so consume the next character.
                m_input >> look;
                t = new OperatorToken("==");
                break;
            }
            return new OperatorToken("=");
            break;
        case '*':
            return new OperatorToken("*");
            break;
        case '.':
            return new OperatorToken(".");
            break;
        default:
            break;
        }

        return t;
    }

} // LCOV_EXCL_LINE