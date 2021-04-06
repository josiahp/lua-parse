// LCOV_EXCL_START
#include <iostream>

#include "../src/lexer.h"
#include "../src/token.h"
#include "gtest/gtest.h"



TEST(Lexer, DefaultConstructor)
{
    std::stringstream helloWorld{"hello world"};
    lua::Lexer lexer{helloWorld};
    EXPECT_TRUE(lexer.hasTokens());
}

TEST(Lexer, StringToken) {
    std::stringstream helloWorld{"hello world "};
    lua::Lexer lexer{helloWorld};
    EXPECT_EQ(lexer.getToken()->ToString(), lua::StringToken("hello").ToString());
    EXPECT_EQ(lexer.getToken()->ToString(), lua::StringToken("world").ToString());
}

TEST(Lexer, NumberToken) {
    std::stringstream helloWorld{" 1 23 "};
    lua::Lexer lexer{helloWorld};
    EXPECT_EQ(lexer.getToken()->ToNumber(), lua::NumberToken(1).ToNumber());
    EXPECT_EQ(lexer.getToken()->ToNumber(), lua::NumberToken(23).ToNumber());
}

TEST(Lexer, EndOfTokens)
{
    std::stringstream helloWorld{"hello world"};
    lua::Lexer lexer{helloWorld};
    EXPECT_TRUE(lexer.hasTokens());
    EXPECT_EQ(lexer.getToken()->ToString(), lua::StringToken("hello").ToString());
    EXPECT_EQ(lexer.getToken()->ToString(), lua::StringToken("world").ToString());
    EXPECT_EQ(lexer.getToken()->Type(), lua::TOKEN_NULL);
}
// LCOV_EXCL_STOP