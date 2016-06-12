#include <memory>
#include <iostream>
#include <vector>
#include <cmath>
#include <thread>
#include <chrono>
#include <mutex>

#include "Sources/Source.h"
#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Parser/AST/PrettyPrintVisitor.h"
#include "Interpreter/Interpreter.h"
#include "Turtle/Turtle.h"

#include "SFML/Graphics.hpp"

std::shared_ptr<Parser::AST::Node> loadSource(std::string sourceFilename)
{
    try {
        std::shared_ptr<Sources::Source> source = std::make_shared<Sources::FileSource>(sourceFilename);

        Lexer::Lexer lexer(source);

        for (auto token : *lexer.getTokens()) {
            std::cout << token.toString() << std::endl;
        }

        Parser::CheloParser parser(lexer.getTokens());

        auto ast = std::dynamic_pointer_cast<Parser::AST::CallNode>(parser.parse());

        Parser::AST::PrettyPrintVisitor ppVisitor;

        ast->acceptVisitor(&ppVisitor);

        std::cout << std::endl << "===" << std::endl;

        return ast;
    }
    catch (std::runtime_error e) {
        std::cout << "U DUN GOOF'D: " << e.what() << std::endl;
        throw e;
    }
    catch (std::exception e) {
        std::cout << "ULTIMATE FAIL: " << e.what() << std::endl;
        throw e;
    }
}

int main(int argc, char *argv[])
{
    sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Hello SFML");

    auto interpreter = Interpreter::Interpreter::makeInterpreter();

    std::thread turtleInterpreter([&]() {
        auto ast = loadSource(argv[1]);

        interpreter->interpret(ast);
    });

    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        {
            auto turtles = interpreter->getTurtles();

            for (auto pair : turtles) {
                auto turtle = pair.second;

                for (auto line : turtle->getLines()) {
                    window.draw(line);
                }
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        window.display();
    }

    int dummy;

    std::cin >> dummy;

    return 0;
}
