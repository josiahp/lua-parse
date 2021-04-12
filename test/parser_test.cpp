// LCOV_EXCL_START
#include <iostream>
#include <string>

#include "../src/lexer.h"
#include "../src/token.h"
#include "../src/parser.h"
#include "gtest/gtest.h"

TEST(Parser, StringExpressionEquality) {
    EXPECT_EQ(lua::StringExpressionNode("foo"), lua::StringExpressionNode("foo"));
}

TEST(Parser, FunctionCallStatementEquality) {
    EXPECT_EQ(
        lua::FunctionCallStatementNode("foo", std::vector<lua::ExpressionNode *>{ new lua::StringExpressionNode("bar") }),
        lua::FunctionCallStatementNode("foo", std::vector<lua::ExpressionNode *>{ new lua::StringExpressionNode("bar") })
    );
}

TEST(Parser, ParseStatementFunctionCallWithExpressionList)
{
    std::stringstream helloWorld{"print(\"Hello\", \"World\")"};
    lua::Lexer lexer{helloWorld};
    lua::Parser parser{lexer};

    lua::ProgramNode *p = parser.Parse();

    lua::ProgramNode *expected = new lua::ProgramNode{
        {
        (lua::StatementNode *) new lua::FunctionCallStatementNode{
            "print",
            std::vector<lua::ExpressionNode *>{
                new lua::StringExpressionNode("Hello"),
                new lua::StringExpressionNode("World")
            },
        }}
    };

    EXPECT_EQ(*p, *expected);
}

TEST(Parser, ParseStatementFunctionCallWithStringLiteral)
{
    std::stringstream helloWorld{"print \"Hello World\""};
    lua::Lexer lexer{helloWorld};
    lua::Parser parser{lexer};

    lua::ProgramNode *p = parser.Parse();

    lua::ProgramNode *expected = new lua::ProgramNode{
        {
        (lua::StatementNode *) new lua::FunctionCallStatementNode{
            "print",
            std::vector<lua::ExpressionNode *>{
                new lua::StringExpressionNode("Hello World")
            },
        }}
    };

    EXPECT_EQ(*p, *expected);
}


// LCOV_EXCL_STOP