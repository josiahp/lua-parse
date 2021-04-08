#pragma once

#include <vector>

#include "lexer.h"

namespace lua
{
    enum NodeType
    {
        NODE_NULL,
        NODE_PROGRAM,
        NODE_STATEMENT,
        NODE_STATEMENT_FUNCTIONCALL,
        NODE_EXPRESSION,
        NODE_EXPRESSION_STRING,
    };

    class Node
    {
    public:
    virtual ~Node() = 0;
        virtual NodeType Type() const = 0;
        virtual std::string TypeAsString() const = 0;
        virtual std::string ToString() const = 0;
        virtual std::string ToString(int depth) const = 0;
    };

    class TerminalNode : public Node
    {
        virtual NodeType Type() const = 0;
        virtual std::string TypeAsString() const = 0;

    };

    class NonTerminalNode : public Node
    {
        virtual NodeType Type() const = 0;
        virtual std::string TypeAsString() const = 0;

    };

    class StatementNode : public TerminalNode
    {
        public:
        virtual NodeType Type() const = 0;
                virtual std::string TypeAsString() const = 0;
        virtual std::string ToString() const = 0;
                virtual std::string ToString(int depth) const = 0;
        
        //virtual NodeType Type() const = 0;
    };

    class ProgramNode : public NonTerminalNode
    {
    public:
        NodeType Type() const override { return NODE_PROGRAM; }
        std::string TypeAsString() const override { return "Program"; }

        void Append(StatementNode *s);
        std::vector<StatementNode *> Body() const { return m_statementNodes; }

        std::string ToString() const { this->ToString(1); }
        std::string ToString(int depth = 1) const;

    private:
        std::vector<StatementNode *> m_statementNodes;
    };

    class ExpressionNode : public NonTerminalNode
    {
        public:
        virtual NodeType Type() const = 0;
                virtual std::string TypeAsString() const = 0;
        virtual std::string ToString() const = 0;
        virtual std::string ToString(int depth) const = 0;
    };

    class StringExpressionNode : public ExpressionNode
    {
        public:
        NodeType Type() const override { return NODE_EXPRESSION_STRING; }
                std::string TypeAsString() const override { return "StringExpression"; }

        std::string ToString() const override { return this->ToString(1); }
        std::string ToString(int depth = 1) const override;

        StringExpressionNode(std::string value) : value(value) {};

        const std::string value;
    };

    class FunctionCallExpressionNode : public ExpressionNode
    {

    public:
        std::string prefixExpression;

        NodeType Type() const override { return NODE_NULL; }

        FunctionCallExpressionNode(std::string prefixExp) : prefixExpression(prefixExp){};
    };

    class FunctionCallStatementNode : public StatementNode
    {

    public:
        std::string prefixExpression;
        std::vector<ExpressionNode*> args;

        NodeType Type() const override { return NODE_STATEMENT_FUNCTIONCALL; }
                std::string TypeAsString() const override { return "FunctionCallStatement"; }

        std::string ToString() const override { return this->ToString(1); }
        std::string ToString(int depth = 1) const override;

        FunctionCallStatementNode(std::string prefixExp, std::vector<ExpressionNode*> args) : prefixExpression(prefixExp), args(args){};
        FunctionCallStatementNode(std::string prefixExp) : prefixExpression(prefixExp) {};

    };

    class Parser
    {
    public:
        Parser(Lexer &lexer) : m_lexer(lexer){};
        ProgramNode *Parse();

        StatementNode* ReadStatementNode();
        FunctionCallStatementNode *ReadFunctionCallStatementNode();
        ExpressionNode* ReadExpressionNode();

    private:
        Lexer &m_lexer;
    };
}