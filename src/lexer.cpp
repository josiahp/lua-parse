#include <iostream>

#include "lexer.h"

namespace lua
{

    Lexer::Lexer(std::iostream &input) : m_input(input) {}

    bool Lexer::hasTokens()
    {
        return m_input.good();
    }

    Token *Lexer::getToken()
    {

        char c;

        m_input >> std::skipws >> c;

        if (std::isalpha(c))
        {
            std::string buffer;

            while (m_input.good() && std::isalpha(c))
            {
                buffer += c;
                m_input >> std::noskipws >> c;
            }
            if (m_input.good())
                m_input.putback(c);

            return new StringToken(buffer);
        }
        else if (std::isdigit(c))
        {

            m_input.putback(c);
            uint64_t value;
            m_input >> std::noskipws >> value;
            return new NumberToken(value);
        }

        while(m_input.good() && std::isspace(m_input.peek())) {
            m_input >> std::noskipws >> c;
        }

        return new NullToken();
    }

} // LCOV_EXCL_LINE