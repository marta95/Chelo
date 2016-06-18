#pragma once

#include <iostream>
#include <sstream>

#include "../Parser/AST/Visitor.h"
#include "../Parser/AST/AST.h"

namespace Interpreter {
    struct InterpreterValue;
    class InterpreterState;

    class InterpreterVisitor : public Parser::AST::Visitor<std::shared_ptr<InterpreterValue>, Parser::AST::Node>
    {
            // InterpreterState &interpreterState;

        public:
            InterpreterState &interpreterState;

            InterpreterVisitor(InterpreterState &);

            std::shared_ptr<InterpreterValue> visit(Parser::AST::Node *) override;

            std::shared_ptr<InterpreterValue> __visit(Parser::AST::NameLiteralNode *nameLiteral);
            std::shared_ptr<InterpreterValue> __visit(Parser::AST::StringLiteralNode *stringLiteral);
            std::shared_ptr<InterpreterValue> __visit(Parser::AST::NumberLiteralNode *numberLiteral);
            std::shared_ptr<InterpreterValue> __visit(Parser::AST::CallNode *call);
            std::shared_ptr<InterpreterValue> __visit(Parser::AST::VectorNode *vector);
    };
}