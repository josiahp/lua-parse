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
        //virtual std::string ToString() const = 0;
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
        virtual std::string ToString(int depth = 0) const = 0;
        virtual inline bool operator==(const StatementNode &rhs) const = 0;
    };

    class ProgramNode : public NonTerminalNode
    {
    public:
        ProgramNode(std::vector<StatementNode *> statements = std::vector<StatementNode *>()) : m_statementNodes(statements){};

        NodeType Type() const override { return NODE_PROGRAM; }
        std::string TypeAsString() const override { return "Program"; }

        void Append(StatementNode *s);
        std::vector<StatementNode *> Body() const { return m_statementNodes; }

        std::string ToString(int depth = 0) const;

        inline friend std::ostream &operator<<(std::ostream &os, const ProgramNode &p)
        {
            return os << p.ToString();
        }
        inline friend bool operator==(const ProgramNode &lhs, const ProgramNode &rhs)
        {
              return std::equal(lhs.m_statementNodes.cbegin(), lhs.m_statementNodes.cend(), rhs.m_statementNodes.cbegin(), [](const StatementNode *lhs, const StatementNode *rhs) { return *lhs == *rhs; });
        }

    private:
        std::vector<StatementNode *> m_statementNodes;
    };

    class ExpressionNode : public NonTerminalNode
    {
    public:
        virtual NodeType Type() const = 0;
        virtual std::string TypeAsString() const = 0;
        virtual std::string ToString(int depth = 0) const = 0;
        virtual inline bool operator==(const ExpressionNode &rhs) const = 0;
    };

    class StringExpressionNode : public ExpressionNode
    {
    public:
        NodeType Type() const override { return NODE_EXPRESSION_STRING; }
        std::string TypeAsString() const override { return "StringExpression"; }

        std::string ToString(int depth = 0) const override;

        StringExpressionNode(std::string value) : value(value){};

        const std::string value;

        inline bool operator==(const ExpressionNode &rhs) const
        {
            return this->Type() == rhs.Type() && this->value == dynamic_cast<const StringExpressionNode &>(rhs).value;
        }
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
        std::vector<ExpressionNode *> args;

        NodeType Type() const override { return NODE_STATEMENT_FUNCTIONCALL; }
        std::string TypeAsString() const override { return "FunctionCallStatement"; }
        std::string ToString(int depth = 1) const override;

        FunctionCallStatementNode(std::string prefixExp, std::vector<ExpressionNode *> args) : prefixExpression(prefixExp), args(args){};
        FunctionCallStatementNode(std::string prefixExp) : prefixExpression(prefixExp){};

        void AppendArg(ExpressionNode *arg)
        {
            args.push_back(arg);
        }

        inline friend std::ostream &operator<<(std::ostream &os, const FunctionCallStatementNode &f)
        {
            return os << f.ToString();
        }

        inline bool operator==(const StatementNode &rhs) const override
        {
            return this->prefixExpression == dynamic_cast< const FunctionCallStatementNode& >(rhs).prefixExpression
              && std::equal(this->args.cbegin(), this->args.cend(), dynamic_cast< const FunctionCallStatementNode& >(rhs).args.cbegin(), [](const ExpressionNode *lhs, const ExpressionNode *rhs) { return *lhs == *rhs; });
        }
    };

    class Parser
    {
    public:
        Parser(Lexer &lexer) : m_lexer(lexer){};
        ProgramNode *Parse();

        StatementNode *ReadStatementNode();
        FunctionCallStatementNode *ReadFunctionCallStatementNode();
        ExpressionNode *ReadExpressionNode();

    private:
        Lexer &m_lexer;
    };
}