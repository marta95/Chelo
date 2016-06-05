#pragma once

#include <memory>
#include <vector>

#include "../AST/AST.h"
#include "../Parser.h"

#include "../../Utilities/optional.h"

namespace Parser
{
    namespace Parselets
    {
        using namespace std::experimental;

        class Parselet
        {
            public:
                virtual optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser) = 0;
        };

        class CallParselet : public Parselet
        {
            public:
                optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser);
        };

        class CommentParselet : public Parselet
        {
            public:
                optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser);
        };

        class NameParselet : public Parselet
        {
        public:
            optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser);
        };

        class StringParselet : public Parselet
        {
        public:
            optional<std::shared_ptr<AST::Node>> parse(::Parser::Parser &parser);
        };
    }
}