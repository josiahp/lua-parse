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

        return p;
    }

    void ProgramNode::Append(StatementNode *s)
    {
        m_statementNodes.push_back(s);
    }

    StatementNode *Parser::ReadStatementNode()
    {
        Token *identifier;

        // All statements begin with an identifier (?), so we start there
        if (m_lexer.PeekNextToken()->Type() != TOKEN_IDENTIFIER)
            return nullptr;

        identifier = m_lexer.GetNextToken();

        // We have a valid identifier

        Token *t = m_lexer.PeekNextToken();
        switch (t->Type())
        {

        case TOKEN_LPAREN: // it's a void function call with an expression list
        {
            FunctionCallStatementNode *f = new FunctionCallStatementNode(identifier->ToString());

            m_lexer.GetNextToken(); // throw away the lparen

            while (m_lexer.HasTokens())
            {
                ExpressionNode *e = ReadExpressionNode();

                // We found a non-expression token
                if (e == nullptr)
                {
                    if (m_lexer.PeekNextToken()->Type() == TOKEN_COMMA)
                    {
                        // consume the comma and keep processing
                        m_lexer.GetNextToken();
                        continue;
                    }
                    else if (m_lexer.PeekNextToken()->Type() == TOKEN_RPAREN)
                    {
                        // consume the rparen
                        m_lexer.GetNextToken();
                        break;
                    }
                }

                f->AppendArg(e);
            }

            return f;
        }

        case TOKEN_STRING: // void function call with string argument
        {
            // TODO(josiah.purtlebaugh@gmail.com): implement table constructors for void function calls
            // case TOKEN_LBRACE: // void function call with table argument
            ExpressionNode *e = ReadExpressionNode();
            FunctionCallStatementNode *f = new FunctionCallStatementNode(
                identifier->ToString(),
                std::vector<ExpressionNode *>{e});
            return f;
        }

        default:
            break;
        }

        return nullptr;
    }

    FunctionCallStatementNode *Parser::ReadFunctionCallStatementNode()
    {
        __builtin_unreachable();
        return nullptr;
    }

    ExpressionNode *Parser::ReadExpressionNode()
    {
        if (m_lexer.PeekNextToken() == nullptr)
            return nullptr;

        switch (m_lexer.PeekNextToken()->Type())
        {
        case TOKEN_STRING:
            return new StringExpressionNode(m_lexer.GetNextToken()->ToString());
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
        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent << "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent << "\"body\": ";

        if (this->m_statementNodes.empty())
        {
            buffer << "{}" << std::endl;
        }
        else
        {
            buffer << std::endl;

            for (StatementNode *s : this->m_statementNodes)
            {
                buffer << s->ToString(depth + 1);
            }
        }

        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "}" << std::endl;

        return buffer.str();
    }

    std::string FunctionCallStatementNode::ToString(int depth) const
    {
        std::stringstream buffer;
        std::string indent = std::string(depth * INDENT_WIDTH, ' ');
        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent << "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent << "\"name\": \"" << this->prefixExpression << "\"," << std::endl;
        buffer << indent << "\"args\": ";

        for (auto it = this->args.cbegin(); it < this->args.cend(); ++it) {
            if (it == this->args.cbegin())
                buffer << std::endl << indent << "[" << std::endl;

            buffer << (*it)->ToString(depth + 2);

            if (it < this->args.cend()-1)
                buffer << ",";

            buffer << std::endl;
        }

        buffer << indent << "]" << std::endl;
        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "}" << std::endl;
        return buffer.str();
    }

    std::string StringExpressionNode::ToString(int depth) const
    {
        std::stringstream buffer;
        std::string indent = std::string(depth * INDENT_WIDTH, ' ');
        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "{" << std::endl;
        buffer << indent << "\"type\": \"" << this->TypeAsString() << "\"," << std::endl;
        buffer << indent << "\"value\": \"" << this->value << "\"" << std::endl;
        buffer << std::string((depth - 1) * INDENT_WIDTH, ' ') << "}";
        return buffer.str();
    }
}