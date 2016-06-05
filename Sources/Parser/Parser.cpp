#include "Parser.h"

#include "../Utilities/optional.h"

#include "Parselets/Parselets.h"

using namespace std::experimental;

namespace Parser
{
    Parser::Parser(std::shared_ptr<std::vector<Lexer::Token>> tokens) :
        tokens(tokens), genericParseletMap(), specificParseletMap()
    {

    }

    Parser &Parser::registerParselet(Lexer::TokenType tokenType,
                                     std::shared_ptr<Parselets::Parselet> parselet)
    {
        genericParseletMap[tokenType] = parselet;

        return *this;
    }

    Parser &Parser::registerParselet(Lexer::TokenType tokenType, std::string content,
                                     std::shared_ptr<Parselets::Parselet> parselet)
    {
        specificParseletMap[std::make_tuple(tokenType, content)] = parselet;

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
            return token.type == tokenType && (tokenContent.empty() || token.content == tokenContent);
        }
        else {
            return false;
        }
    }

    std::shared_ptr<Parselets::Parselet> Parser::getParselet(Lexer::Token token)
    {
        Lexer::TokenType tokenType = token.type;
        std::string &content = token.content;

        auto specificParselet = specificParseletMap.find(std::make_tuple(tokenType, content));
        auto genericParselet = genericParseletMap.find(tokenType);

        if (specificParselet != specificParseletMap.end()) {
            return specificParselet->second;
        }
        else if (genericParselet != genericParseletMap.end()) {
            return genericParselet->second;
        }
        else {
            throw std::runtime_error("Can't parse token: " + token.toString());
        }
    }

    optional<std::shared_ptr<AST::Node>> Parser::parseExpression()
    {
        optional<Lexer::Token> optionalToken = peekToken();
        
        if (optionalToken) {
            auto token = optionalToken.value();
            auto parselet = getParselet(token);

            if (parselet != nullptr) {
                return parselet->parse(*this);
            }
            else {
                throw std::runtime_error("n't parse token: " + token.toString());
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
            Lexer::Token token = tokens->at(position);
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
        this->registerParselet(Lexer::TokenType::OpenParen, "(", std::make_shared<Parselets::CallParselet>());
        this->registerParselet(Lexer::TokenType::Comment, std::make_shared<Parselets::CommentParselet>());
        this->registerParselet(Lexer::TokenType::Name, std::make_shared<Parselets::NameParselet>());
        this->registerParselet(Lexer::TokenType::String, std::make_shared<Parselets::StringParselet>());
    }
}