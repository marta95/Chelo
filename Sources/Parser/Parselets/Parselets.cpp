#include "Parselets.h"

namespace Parser
{
    namespace Parselets
    {
        using namespace std::experimental;

        optional<std::shared_ptr<AST::Node>> CallParselet::parse(Parser &parser)
        {
            auto openParenToken = parser.consume(Lexer::TokenType::OpenParen, "(");

            auto parameters = std::make_shared<std::vector<std::shared_ptr<AST::Node>>>();

            auto functionNameOptional = parser.parseExpression();
            
            if (!functionNameOptional) {
                throw std::runtime_error("Call form " + openParenToken.toString() + " requires at least one form.");
            }

            do
            {
                auto optionalExpression(parser.parseExpression());
                if (optionalExpression) {
                    parameters->push_back(optionalExpression.value());
                } else {
                    throw std::runtime_error("Unexpected end of form starting at: " + openParenToken.toString());
                }
            } while ((parser.match(Lexer::TokenType::CloseParen, ")")));

            parser.consume(Lexer::TokenType::CloseParen, ")");

            return make_optional<std::shared_ptr<AST::Node>>(std::make_shared<AST::CallNode>(functionNameOptional.value(), parameters));
        }

        optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser)
        {
            parser.consume(Lexer::TokenType::Comment);
            return nullopt;
        }
    }
}