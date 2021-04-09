#include <iostream>
#include <string>
#include <sstream>

#include "lexer.h"
#include "token.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    std::stringstream helloWorld{"print(\"Hello\", \"World\")"};
    lua::Lexer lexer{helloWorld};
    lua::Parser parser{lexer};

    lua::ProgramNode *p = parser.Parse();

    std::cout << p->ToString() << std::endl;
}