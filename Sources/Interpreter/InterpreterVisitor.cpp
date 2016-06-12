#include "InterpreterVisitor.h"

namespace Interpreter {
    InterpreterVisitor::InterpreterVisitor(InterpreterState &interpreterState) :
        interpreterState(interpreterState)
    {

    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::visit(Parser::AST::Node *whatever)
    {
        if (dynamic_cast<Parser::AST::NameLiteralNode *>(whatever)) {
            Parser::AST::NameLiteralNode *derp = dynamic_cast<Parser::AST::NameLiteralNode *>(whatever);

            return this->__visit(derp);
        }
        else if (dynamic_cast<Parser::AST::StringLiteralNode *>(whatever)) {
            Parser::AST::StringLiteralNode *derp = dynamic_cast<Parser::AST::StringLiteralNode *>(whatever);

            return this->__visit(derp);
        }
        else if (dynamic_cast<Parser::AST::NumberLiteralNode *>(whatever)) {
            Parser::AST::NumberLiteralNode *derp = dynamic_cast<Parser::AST::NumberLiteralNode *>(whatever);

            return this->__visit(derp);
        }
        else if (dynamic_cast<Parser::AST::CallNode *>(whatever)) {
            Parser::AST::CallNode *derp = dynamic_cast<Parser::AST::CallNode *>(whatever);

            return this->__visit(derp);
        }
        else if (dynamic_cast<Parser::AST::VectorNode *>(whatever)) {
            Parser::AST::VectorNode *derp = dynamic_cast<Parser::AST::VectorNode *>(whatever);

            return this->__visit(derp);
        }
        else {
            throw std::runtime_error("ugh");
        }
    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::__visit(Parser::AST::NameLiteralNode *nameLiteral)
    {
        auto value = interpreterState.lookup(nameLiteral->name);

        if (value) {
            return value.value();
        } else {
            return std::make_shared<Interpreter::NilValue>();
        }
    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::__visit(Parser::AST::StringLiteralNode *stringLiteral)
    {
        return std::make_shared<Interpreter::StringValue>(stringLiteral->content);
    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::__visit(Parser::AST::NumberLiteralNode *numberLiteral)
    {
        return std::make_shared<Interpreter::NumberValue>(numberLiteral->value);
    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::__visit(Parser::AST::CallNode *call)
    {
        // auto result = std::make_shared<Interpreter::NilValue>();
        auto functionNameResult = call->functionName->acceptVisitor(this);

        auto castFunctionName = std::dynamic_pointer_cast<FunctionValue>(functionNameResult);

        auto parameters = std::make_shared<FunctionParameters>();
            
        for (auto param : *call->parameters) {
            auto result = param->acceptVisitor(this);
            parameters->push_back(result);
        }

        if (castFunctionName) {
            return castFunctionName->call(interpreterState, *parameters);
        }
        else {
            throw std::runtime_error("Expected a function, got: " + functionNameResult->stringRepr());
        }
    }

    std::shared_ptr<InterpreterValue> InterpreterVisitor::__visit(Parser::AST::VectorNode *vector)
    {
        return std::make_shared<Interpreter::NilValue>();
    }

}