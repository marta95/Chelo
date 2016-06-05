#include <memory>
#include <iostream>

#include "Sources/Source.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"

int main(int argc, char *argv[])
{
    try {
        std::shared_ptr<Sources::Source> source = std::make_shared<Sources::FileSource>(argv[1]);

        Lexer::Lexer lexer(source);

        for (auto token : *lexer.getTokens()) {
            std::cout << token.toString() << std::endl;
        }

        Parser::CheloParser parser(lexer.getTokens());

        auto ast = parser.parse();

        std::cout << ast->toString() << std::endl;

    }
    catch (std::runtime_error e) {
        std::cout << "U DUN GOOF'D: " << e.what() << std::endl;
    }
    catch (std::exception e) {
        std::cout << "ULTIMATE FAIL: " << e.what() << std::endl;
    }

    int dummy;

    std::cin >> dummy;

    return 0;
}
