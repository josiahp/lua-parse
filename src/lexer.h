#pragma once

#include <iostream>

#include "token.h"

namespace lua
{
    class Lexer
    {
    public:
        Lexer(std::iostream &input);
        Token* getToken();
        bool hasTokens();

    private:


        std::iostream &m_input;
        //Token* m_currentToken;
    };
}