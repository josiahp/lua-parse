// LCOV_EXCL_START
#include <iostream>
#include <string>

#include "../src/lexer.h"
#include "../src/token.h"
#include "gtest/gtest.h"

TEST(Lexer, DefaultConstructor)
{
    std::stringstream helloWorld{"hello world"};
    lua::Lexer lexer{helloWorld};
    EXPECT_TRUE(lexer.hasTokens());
}

TEST(Lexer, IdentifierToken)
{
    std::stringstream helloWorld{"hello world "};
    lua::Lexer lexer{helloWorld};

    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_IDENTIFIER);
    EXPECT_EQ(t->ToString(), std::string("hello"));

    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_IDENTIFIER);
    EXPECT_EQ(t->ToString(), std::string("world"));
}

TEST(Lexer, NumberToken)
{
    std::stringstream helloWorld{" 1 23 "};
    lua::Lexer lexer{helloWorld};
    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_NUMBER);
    EXPECT_EQ(t->ToNumber(), 1);

    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_NUMBER);
    EXPECT_EQ(t->ToNumber(), 23);
}

TEST(Lexer, StringToken)
{
    std::stringstream helloWorld{" \"hello world\" "};
    lua::Lexer lexer{helloWorld};
    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_STRING);
    EXPECT_EQ(t->ToString(), std::string("hello world"));
}

TEST(Lexer, EndOfTokens)
{
    std::stringstream helloWorld{" hello world1 123 "};
    lua::Lexer lexer{helloWorld};

    EXPECT_TRUE(lexer.hasTokens());

    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_IDENTIFIER);
    EXPECT_EQ(t->ToString(), std::string("hello"));

    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_IDENTIFIER);
    EXPECT_EQ(t->ToString(), std::string("world1"));

    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_NUMBER);
    EXPECT_EQ(t->ToNumber(), 123);

    EXPECT_FALSE(lexer.hasTokens());
}

TEST(Lexer, EmptyStream)
{
    std::stringstream helloWorld{""};
    lua::Lexer lexer{helloWorld};

    EXPECT_FALSE(lexer.hasTokens());

    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_NULL);
}

TEST(Lexer, Parens)
{
    std::stringstream helloWorld{"hello (world)"};
    lua::Lexer lexer{helloWorld};

    // "hello"
    lua::Token *t = lexer.getToken();

    // "("
    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_LPAREN);

    // "world"
    t = lexer.getToken();

    // ")"
    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_RPAREN);
}

TEST(Lexer, HelloWorld)
{
    std::stringstream helloWorld{"print(\"Hello World\")"};
    lua::Lexer lexer{helloWorld};

    // "print"
    lua::Token *t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_IDENTIFIER);
    EXPECT_EQ(t->ToString(), std::string("print"));

    // "("
    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_LPAREN);

    // "\"Hello World\""
    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_STRING);
    EXPECT_EQ(t->ToString(), std::string("Hello World"));

    // ")"
    t = lexer.getToken();
    EXPECT_EQ(t->Type(), lua::TOKEN_RPAREN);
}

TEST(Lexer, FactorialFunction)
{
    std::stringstream factorial{R"(
    -- defines a factorial function
    function fact (n)
      if n == 0 then
        return 1
      else
        return n * fact(n-1)
      end
    end

    print("enter a number:")
    a = io.read("*number")      -- read a number
    print(fact(a))
    )"};
    lua::Lexer lexer{factorial};

    lua::Token *expectedTokens[] = {
        new lua::CommentToken("defines a factorial function"),

        new lua::IdentifierToken("function"),
        new lua::IdentifierToken("fact"),
        new lua::LParenToken(),
        new lua::IdentifierToken("n"),
        new lua::RParenToken(),

        new lua::IdentifierToken("if"),
        new lua::IdentifierToken("n"),

        new lua::OperatorToken("=="),
        new lua::NumberToken(0),
        new lua::IdentifierToken("then"),
        new lua::IdentifierToken("return"),
        new lua::NumberToken(1),
        new lua::IdentifierToken("else"),
        new lua::IdentifierToken("return"),
        new lua::IdentifierToken("n"),
        new lua::OperatorToken("*"),
        new lua::IdentifierToken("fact"),
        new lua::LParenToken(),
        new lua::IdentifierToken("n"),
        new lua::OperatorToken("-"),
        new lua::NumberToken(1),
        new lua::RParenToken(),
        new lua::IdentifierToken("end"),
        new lua::IdentifierToken("end"),

        new lua::IdentifierToken("print"),
        new lua::LParenToken(),
        new lua::StringToken("enter a number:"),
        new lua::RParenToken(),

        new lua::IdentifierToken("a"),
        new lua::OperatorToken("="),
        new lua::IdentifierToken("io"),
        new lua::OperatorToken("."),
        new lua::IdentifierToken("read"),
        new lua::LParenToken(),
        new lua::StringToken("*number"),
        new lua::RParenToken(),
        new lua::CommentToken("read a number"),

        new lua::IdentifierToken("print"),
        new lua::LParenToken(),
        new lua::IdentifierToken("fact"),
        new lua::LParenToken(),
        new lua::IdentifierToken("a"),
        new lua::RParenToken(),
        new lua::RParenToken(),

    };

    EXPECT_TRUE(lexer.hasTokens());

    for (int i = 0; lexer.hasTokens(); ++i)
    {
        lua::Token *t = lexer.getToken();
        EXPECT_EQ(t->Type(), expectedTokens[i]->Type());
        EXPECT_EQ(t->ToString(), expectedTokens[i]->ToString());
    }
}

// LCOV_EXCL_STOP