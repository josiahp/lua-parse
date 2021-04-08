#include <iostream>
#include <string>
#include <sstream>

#include "lexer.h"
#include "token.h"
#include "parser.h"

//namespace lua
//{
/*
    class NumericLiteral
    {
    public:
        NumericLiteral(const int16_t& n) : m_value(n){};

        auto value() const -> const int16_t & { return m_value; }

        auto toString() const -> const std::string
        {
            std::stringstream buffer;
            buffer
                << "  {" << std::endl
                << "    \"type\": \"" << type() << "\"," << std::endl
                << "    \"value\": \"" << m_value << "\"" << std::endl
                << "  }";

            return buffer.str();
        }

        static inline std::string type() { return "NumericLiteral"; }

    private:
        const int16_t m_value;
    };

    class Program
    {

    public:
        Program(const NumericLiteral &body) : m_body(body){};
        auto toString() const -> const std::string
        {
            std::stringstream buffer;
            buffer
                << "{" << std::endl
                << "  \"type\": \"" << type() << "\"," << std::endl
                << "  \"body\": " << std::endl
                << m_body.toString() << std::endl
                << "}";

            return buffer.str();
        }

        static inline std::string type() { return "Program"; }

    private:
        const NumericLiteral &m_body;
    };

    class Token {
        public:
        virtual const std::string type() const;
        virtual bool validate(char c) const;
    };

    class NullToken : public Token {
        public:
        inline std::string type() { return "NullToken"; }
        bool validate(char c) {
            return false;
        }
    };

    class Number : public Token {
        public:
        inline std::string type() { return "NumberToken"; }
        const int value;

        Number(const std::string &data) : value(std::stoi(data)) {}
        bool validate(char c) {
            return c >= '0' && c <= '9';
        }
    };

    class Tokenizer {
        public:
        const Token *currentToken;

        Tokenizer(const std::string &data) : m_data(data), m_dataIt(m_data.cbegin()) {}
        const Token getToken() {
            std::string token;

            for (; m_dataIt <= m_data.cend(); ++m_dataIt) {
                if (*m_dataIt >= '0' && *m_dataIt <= '9') {
                    token += *m_dataIt;
                } else break;
            }

            if (token.length() == 0) {
                return NullToken();
            }

            return Number(token);
        }

        private:
        const std::string m_data;
        std::string::const_iterator m_dataIt;
    };


    class Parser
    {
    public:
        Parser(const std::string &program) : m_program(program), m_programIt(m_program.cbegin()) {}
        auto parse() -> const Program &
        {
            Tokenizer tok{m_program};
            const Token t;

            for (t = tok.getToken(); t != NULL; t = tok.getToken()) {
                if (t->type() == "NumberToken") {

                }
            }

            const Program &p{
                NumericLiteral(std::stoi(m_program))
            };

            return p;
        }

    private:
        const std::string m_program;
        std::string::const_iterator m_programIt;
    };
};
*/



int main(int argc, char *argv[])
{
    std::stringstream helloWorld{"print(\"Hello World\")"};
    lua::Lexer lexer{helloWorld};
    lua::Parser parser{lexer};

    lua::ProgramNode *p = parser.Parse();

    std::cout << p->ToString(1) << std::endl;
}