#include "Interpreter.h"

#include "InterpreterVisitor.h"

namespace Interpreter
{
    void Interpreter::interpret(std::shared_ptr<Parser::AST::Node> expression)
    {
        auto visitor = InterpreterVisitor(interpreterState);

        expression->acceptVisitor(&visitor);
    }

    std::shared_ptr<Interpreter> Interpreter::makeInterpreter()
    {
        auto interpreter = std::make_shared<Interpreter>();

        interpreter->interpreterState.registerToplevelFunction("say",
            [](InterpreterState &state, FunctionParameters parameters) {
                auto &output = state.getOutputStream();

                for (auto parameter : parameters) {
                    output << parameter->stringRepr() << " ";
                }
            
                output << std::endl;

                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("str",
            [](InterpreterState &state, FunctionParameters parameters) {
                std::stringstream stringRepr("");

                for (auto parameter : parameters) {
                    stringRepr << parameter->stringRepr() << " ";
                }

                return std::make_shared<StringValue>(stringRepr.str());
            });

        interpreter->interpreterState.registerToplevelFunction("+",
            [](InterpreterState &state, FunctionParameters parameters) {
                double accumulator = 0;

                for (auto parameter : parameters) {
                    auto numberParam = std::dynamic_pointer_cast<NumberValue>(parameter);

                    if (numberParam) {
                        accumulator += numberParam->value;
                    }
                    else {
                        throw std::runtime_error("Expected a number, got: " + parameter->stringRepr());
                    }
                }

                return std::make_shared<NumberValue>(accumulator);
            });

        interpreter->interpreterState.registerToplevelFunction("*",
            [](InterpreterState &state, FunctionParameters parameters) {
            double accumulator = 1;

            for (auto parameter : parameters) {
                auto numberParam = std::dynamic_pointer_cast<NumberValue>(parameter);

                if (numberParam) {
                    accumulator *= numberParam->value;
                }
                else {
                    throw std::runtime_error("Expected a number, got: " + parameter->stringRepr());
                }
            }

            return std::make_shared<NumberValue>(accumulator);
        });

        interpreter->interpreterState.registerToplevelFunction("do",
            [](InterpreterState &state, FunctionParameters parameters) {
                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("defturtle",
            [](InterpreterState &state, FunctionParameters parameters) {
                if (parameters.size() != 1) { throw std::runtime_error("defturtle expects 1 parameter.");  }

                auto turtleName = std::dynamic_pointer_cast<StringValue>(parameters.front());

                if (turtleName) {
                    auto newTurtle = std::make_shared<Turtle::Turtle>();
                    state.addTurtle(turtleName->value, newTurtle);

                    auto turtleValue = std::make_shared<TurtleValue>(newTurtle);

                    state.assignToName(turtleName->value, turtleValue);
                }
                else {
                    throw std::runtime_error("Turtle must be bound to a name.");
                }

                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("move-forward!",
            [](InterpreterState &state, FunctionParameters parameters) {
                if (parameters.size() != 2) { throw std::runtime_error("defturtle expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto distance = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                    if (distance) {
                        turtleDescriptor->moveForward(distance->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to move-forward! must be a number.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to move-forward! must be a turtle.");
                }

                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("rotate-left!",
            [](InterpreterState &state, FunctionParameters parameters) {
                if (parameters.size() != 2) { throw std::runtime_error("defturtle expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto angle = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                    if (angle) {
                        turtleDescriptor->rotateLeft(angle->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to rotate-left! must be a number.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to rotate-leftd! must be a turtle.");
                }

                return std::make_shared<NilValue>();
            });

        // interpreter->interpreterState.push();

        return interpreter;
    }
}