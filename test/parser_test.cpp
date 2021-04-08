// LCOV_EXCL_START
#include <iostream>
#include <string>

#include "../src/lexer.h"
#include "../src/token.h"
#include "../src/parser.h"
#include "gtest/gtest.h"

TEST(Parser, HelloWorldParse)
{
    std::stringstream helloWorld{"print(\"Hello World\")"};
    lua::Lexer lexer{helloWorld};
    lua::Parser parser{lexer};

    lua::ProgramNode *p = parser.Parse();

    ASSERT_NE(p, nullptr);
    EXPECT_EQ(p->Type(), lua::NODE_PROGRAM);

    ASSERT_NE(p->Body().at(0), nullptr);
    EXPECT_EQ(p->Body().size(), 1);
    EXPECT_EQ(p->Body().at(0)->Type(), lua::NODE_STATEMENT_FUNCTIONCALL);

    lua::FunctionCallStatementNode *f = (lua::FunctionCallStatementNode *)p->Body().at(0);

    EXPECT_EQ(f->args.size(), 1);

    std::vector<lua::ExpressionNode *> e = f->args;
    ASSERT_NE(e.at(0), nullptr);
    EXPECT_EQ(e.at(0)->Type(), lua::NODE_EXPRESSION_STRING);

    lua::StringExpressionNode *strexp = (lua::StringExpressionNode *)e.at(0);
    EXPECT_EQ(strexp->value, std::string("Hello World"));
}

// LCOV_EXCL_STOP