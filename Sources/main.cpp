#include <iostream>
#include <memory>
#include <regex>
#include <fstream>

#include "Source.h"
#include "Parser.h"
#include "Interpreter.h"

int main(int argc, char *argv[])
{
    std::shared_ptr<Source> source = std::make_shared<FileSource>(argv[1]);
    Parser parser(source);

    try {
        auto program = parser.getProgram();
        auto interpreter = new Interpreter(program);

        interpreter->interpret();
    } catch (std::exception exception) {
        std::cout << "U DUN GOOF'D: " << exception.what() << std::endl;
    }

    return 0;
}
