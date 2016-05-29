#pragma once

#include <memory>
#include <vector>
#include <map>
#include <tuple>
#include "../Utilities/optional.h"

#include "../Lexer/Lexer.h"
#include "AST/AST.h"
// #include "Parselets/Parselets.h"

namespace Parser
{
    namespace Parselets {
        class Parselet;
    }

    class Parser
    {
            std::shared_ptr<std::vector<Lexer::Token>> tokens;
            std::shared_ptr<AST::Node> parsedAST = nullptr;
            std::map<std::tuple<Lexer::TokenType, std::string>, std::shared_ptr<Parselets::Parselet>> parseletMap;

            int position = 0;
            std::experimental::optional<Lexer::Token> peekToken();

        public:
            Parser(std::shared_ptr<std::vector<Lexer::Token>> tokens);
            Parser &registerParselet(std::tuple<Lexer::TokenType, std::string> id, std::shared_ptr<Parselets::Parselet> parselet);
            std::shared_ptr<AST::Node> parse();
            bool match(Lexer::TokenType tokenType, std::string tokenContent = "");
            std::experimental::optional<std::shared_ptr<AST::Node>> parseExpression();
            Lexer::Token consume(Lexer::TokenType tokenType, std::string tokenContent = "");
    };

    class CheloParser : public Parser
    {
        public:
            CheloParser(std::shared_ptr<std::vector<Lexer::Token>> tokens);
            // std::shared_ptr<AST::Node> parse();
    };
}