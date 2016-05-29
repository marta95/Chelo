#include "Parser.h"

#include "../Utilities/optional.h"

#include "Parselets/Parselets.h"

using namespace std::experimental;

namespace Parser
{
    Parser::Parser(std::shared_ptr<std::vector<Lexer::Token>> tokens) :
        tokens(tokens), parseletMap()
    {

    }

    Parser &Parser::registerParselet(std::tuple<Lexer::TokenType, std::string> id, std::shared_ptr<Parselets::Parselet> parselet)
    {
        parseletMap[id] = parselet;

        return *this;
    }

    std::shared_ptr<AST::CallNode> makeImplicitDo()
    {
        return std::make_shared<AST::CallNode>(std::make_shared<AST::NameLiteralNode>("do"),
                                               std::make_shared<std::vector<std::shared_ptr<AST::Node>>>());
    }

    std::shared_ptr<AST::Node> Parser::parse()
    {
        position = 0;
        std::shared_ptr<AST::CallNode> topLevel = makeImplicitDo();

        while (true) {
            auto topLevelFormOptional = this->parseExpression();

            if (topLevelFormOptional) {
                topLevel->parameters->push_back(topLevelFormOptional.value());
            }
            else {
                break;
            }
        }

        return topLevel;
    }

    bool Parser::match(Lexer::TokenType tokenType, std::string tokenContent)
    {
        optional<Lexer::Token> optionalToken = peekToken();

        if (optionalToken) {
            auto token = optionalToken.value();
            return token.type == tokenType && (token.content.empty() || token.content == tokenContent);
        }
        else {
            return false;
        }
    }

    optional<std::shared_ptr<AST::Node>> Parser::parseExpression()
    {
        optional<Lexer::Token> optionalToken = peekToken();
        
        if (optionalToken) {
            auto token = optionalToken.value();
            auto parselet = parseletMap[std::make_tuple(token.type, token.content)];

            if (parselet != nullptr) {
                return parselet->parse(*this);
            }
            else {
                throw std::runtime_error("Can't parse token:" + token.toString());
            }
        }
        else {
            // throw std::runtime_error("Unexpected end of tokens.");
            return nullopt;
        }
    }

    optional<Lexer::Token> Parser::peekToken()
    {
        if (position < tokens->size()) {
            Lexer::Token token(tokens->at(position));
            return make_optional(token);
        }
        else {
            return nullopt;
        }
    }

    Lexer::Token Parser::consume(Lexer::TokenType tokenType, std::string tokenContent)
    {
        if (match(tokenType, tokenContent)) {
            auto token = peekToken();
            position += 1;

            return token.value();
        }
        else {
            throw std::runtime_error("Unexpected token: " + peekToken().value().toString());
        }
    }

    CheloParser::CheloParser(std::shared_ptr<std::vector<Lexer::Token>> tokens) :
        Parser(tokens)
    {
        this->registerParselet(std::make_tuple(Lexer::TokenType::OpenParen, "("),
                               std::make_shared<Parselets::CallParselet>());
        // this->registerParselet(std::make_tuple)
    }
}