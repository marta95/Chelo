#pragma once

#include "IVisitor.h"
#include "Parser.h"
#include "AST.h"

using namespace std::experimental;

class InterpreterValue
{

};

class InterpreterState
{
    public:
        std::stack<std::map<std::string, InterpreterValue>> stack;

        InterpreterState()
        {
            //
        }

        void stackPush()
        {

        }

        void stackPop()
        {

        }

        InterpreterValue &lookUp(std::string &name)
        {

        }
};

class InterpreterVisitor : public IVisitor
{
    public:
        std::shared_ptr<Value> visit(AST::List &form)
        {
            std::cout << "Visting list: "<< form.to_str() << std::endl;
            auto elements = form.getElements();
            auto firstElement = dynamic_cast<AST::NameLiteral *>(elements[0].get());
            auto rest = std::vector<std::shared_ptr<AST::Form>>(elements.size() - 1);
            std::copy(++elements.begin(), elements.end(), rest.begin());

            if (firstElement->getName() == "do") {
                for(auto doForm : rest) {
                    doForm->acceptVisitor(*this);
                }
            } else if (firstElement->getName() == "say") {
                std::stringstream stringToPrint("");

                for (auto doForm : rest) {
                    std::shared_ptr<Value> result = doForm->acceptVisitor(*this);

                    StringValue *castValue = dynamic_cast<StringValue *>(result.get());

                    if (castValue != nullptr) {
                        stringToPrint << castValue->getValue();
                    } else {
                        throw std::runtime_error("Expected string value, got something else");
                    }
                }

                std::cout << "Say says: "<< stringToPrint.str() << std::endl;
            } else if (firstElement->getName() == "str") {
                std::stringstream stringifiedValue("");

                for (auto doForm : rest) {
                    std::shared_ptr<Value> result = doForm->acceptVisitor(*this);

                    stringifiedValue << result->toString() << " ";
                }

                auto stringReturn = std::make_shared<StringValue>();
                stringReturn->setValue(stringifiedValue.str());

                return stringReturn;
            } else if (firstElement->getName() == "add") {
                int sum = 0;

                for (auto doForm : rest) {
                    auto result = doForm->acceptVisitor(*this);

                    auto castValue = dynamic_cast<IntegerValue *>(result.get());

                    if (castValue != nullptr) {
                        sum += castValue->getValue();
                    } else {
                        throw std::runtime_error("Expected integer value, got something else");
                    }
                }

                auto integerReturn = std::make_shared<IntegerValue>();
                integerReturn->setValue(sum);

                return integerReturn;
            }

            return std::make_shared<NilValue>();
        }

        std::shared_ptr<Value> visit(AST::Vector &form)
        {
            std::cout << "Visting vector: "<< form.to_str() << std::endl;

            return std::make_shared<NilValue>();
        }

        std::shared_ptr<Value> visit(AST::NameLiteral &form)
        {
            std::cout << "Visting name: "<< form.to_str() << std::endl;

            return std::make_shared<NilValue>();
        }

        std::shared_ptr<Value> visit(AST::StringLiteral &form)
        {
            std::cout << "Visting string: "<< form.to_str() << std::endl;

            auto stringValue = std::make_shared<StringValue>();
            stringValue->setValue(form.getText());

            return stringValue;
        }

        std::shared_ptr<Value> visit(AST::IntegerLiteral &form)
        {
            std::cout << "Visting integer: "<< form.to_str() << std::endl;

            auto integerValue = std::make_shared<IntegerValue>();
            integerValue->setValue(form.getValue());

            return integerValue;
        }
};

class Interpreter
{
    std::shared_ptr<Program> program;
    public:
        Interpreter(std::shared_ptr<Program> &program):
            program(program)
        {

        }

        void interpret()
        {
            InterpreterState state;
            InterpreterVisitor visitor;

            auto test = program->getProgram();

            visitor.visit(dynamic_cast<AST::List &>(*test));
        }
};