#include <cassert>
#include <iostream>
#include <string>
#include <sstream>

#include "parser.h"
#include "lexer.h"

#define INDENT_WIDTH 2

namespace lua
{
    Node::~Node() {}

    ProgramNode *Parser::Parse()
    {
        ProgramNode *p = new ProgramNode();

        StatementNode *s = ReadStatementNode();

        p->Append(s);
        /*
        while (m_lexer.hasTokens()) {
            Token *t = m_lexer.getToken();

            switch(t->Type()) {
                case TOKEN_IDENTIFIER:
                    FunctionCallStatementNode *f = new FunctionCallStatementNode(t->ToString());
            }
        }*/

        return p;
    }

    void ProgramNode::Append(StatementNode *s)
    {
        m_statementNodes.push_back(s);
    }

    StatementNode *Parser::ReadStatementNode()
    {
        // All statements begin with an identifier (?), so we start there
        Token *identifier = m_lexer.getToken();
        if (identifier == nullptr || identifier->Type() != TOKEN_IDENTIFIER)
            return nullptr;

        // We have a valid identifier

        Token *t = m_lexer.getToken();
        switch (t->Type())
        {
        case TOKEN_LPAREN:
        { // it's a void function call with an expression list
            ExpressionNode *e = ReadExpressionNode();
            FunctionCallStatementNode *f = new FunctionCallStatementNode(
                identifier->ToString(),
                std::vector<ExpressionNode *>{e});
            m_lexer.getToken();
            // TODO(josiah.purtlebaugh@gmail.com): Don't just assert this, let's do a real check
            //assert(t->Type() == TOKEN_RPAREN);
            return f;
            break;
        }

        case TOKEN_STRING: // void function call with string argument
            // TODO(josiah.purtlebaugh@gmail.com): implement table constructors for void function calls
            // case TOKEN_LBRACE: // void function call with table argument
            return nullptr;
            break;

        default:
            break;
        }

        return nullptr;
    }

    FunctionCallStatementNode *Parser::ReadFunctionCallStatementNode()
    {
        /*Token *t = m_lexer.getToken();

        if (t == nullptr || t->Type() != TOKEN_IDENTIFIER)
            return nullptr;

        FunctionCallStatementNode *f = new FunctionCallStatementNode(t->ToString());

        return f;*/
    }

    ExpressionNode *Parser::ReadExpressionNode()
    {
        Token *t = m_lexer.getToken();

        if (t == nullptr)
            return nullptr;

        switch (t->Type())
        {
        case TOKEN_STRING:
            return new StringExpressionNode(t->ToString());
        default:
            return nullptr;
        }

        // unreachable
        return nullptr;
    }

    std::string ProgramNode::ToString(int depth) const
    {
        std::stringstream buffer;
        std::string indent = std::string(depth * INDENT_WIDTH, ' ');
        buffer <<  std::string((depth-1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent<< "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent<< "\"body\": ";

        for (StatementNode *s : this->m_statementNodes)
        {
            buffer << s->ToString(depth+1);
        }

        if (this->m_statementNodes.empty()) {
            buffer << "{}" << std::endl;
        }

        buffer << std::string((depth-1) * INDENT_WIDTH, ' ') << "}" << std::endl;

        return buffer.str();
    }

    std::string FunctionCallStatementNode::ToString(int depth) const
    {
        std::stringstream buffer;
        std::string indent = std::string(depth * INDENT_WIDTH, ' ');
        buffer <<  std::string((depth-1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent << "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent << "\"name\": \"" << this->prefixExpression << "\"," << std::endl;
        buffer << indent << "\"args\": [";

        for (ExpressionNode *e : this->args)
        {
            buffer << std::endl;
            buffer << e->ToString(depth + 2);
        }

        buffer << indent << "]" << std::endl;
        buffer <<  std::string((depth-1) * INDENT_WIDTH, ' ') << "}" << std::endl;
        return buffer.str();
    }

    std::string StringExpressionNode::ToString(int depth) const
    {
        std::stringstream buffer;
        std::string indent = std::string(depth * INDENT_WIDTH, ' ');
        buffer <<  std::string((depth-1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent << "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent << "\"value\": \"" << this->value << "\"" << std::endl;
        buffer <<  std::string((depth-1) * INDENT_WIDTH, ' ') << "}" << std::endl;
        return buffer.str();
    }
}