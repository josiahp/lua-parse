#pragma once

#include <iostream>

#include "token.h"

namespace lua
{
    class Lexer
    {
    public:
        Lexer(std::iostream &input);
        Token *GetNextToken();
        Token *CurrentToken() const;
        Token *PeekNextToken() const;
        bool HasTokens() const;

    private:
        Token* ParseNextToken();
        StringToken *ReadStringToken(unsigned char);
        IdentifierToken *ReadIdentifierToken(unsigned char);
        NumberToken *ReadNumberToken();
        LParenToken *ReadLParenToken();
        RParenToken *ReadRParenToken();
        CommaToken *ReadCommaToken();
        CommentToken *ReadCommentToken();
        Token *ReadMulticharacterToken(unsigned char);

        std::iostream &m_input;
        Token* m_currentToken;
        Token* m_nextToken;
    };
}