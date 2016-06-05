#include <memory>
#include <iostream>

#include "Sources/Source.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/AST/PrettyPrintVisitor.h"

int main(int argc, char *argv[])
{
    try {
        std::shared_ptr<Sources::Source> source = std::make_shared<Sources::FileSource>(argv[1]);

        Lexer::Lexer lexer(source);

        for (auto token : *lexer.getTokens()) {
            std::cout << token.toString() << std::endl;
        }

        Parser::CheloParser parser(lexer.getTokens());

        auto ast = std::dynamic_pointer_cast<Parser::AST::CallNode>(parser.parse());

        Parser::AST::PrettyPrintVisitor ppVisitor;

        ast->acceptVisitor(&ppVisitor);
    }
    catch (std::runtime_error e) {
        std::cout << "U DUN GOOF'D: " << e.what() << std::endl;
        throw e;
    }
    catch (std::exception e) {
        std::cout << "ULTIMATE FAIL: " << e.what() << std::endl;
        throw e;
    }

    int dummy;

    std::cin >> dummy;

    return 0;
}
