#include "Interpreter.h"

#include <thread>
#include <chrono>

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

        interpreter->interpreterState.registerToplevelFunction("say!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                auto &output = state.getOutputStream();

                for (auto parameter : parameters) {
                    output << parameter->stringRepr() << " ";
                }
            
                output << std::endl;

                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("str",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                std::stringstream stringRepr("");

                for (auto parameter : parameters) {
                    stringRepr << parameter->stringRepr() << " ";
                }

                return std::make_shared<StringValue>(stringRepr.str());
            });

        interpreter->interpreterState.registerToplevelFunction("+",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
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
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
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
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("defturtle",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
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
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
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

        interpreter->interpreterState.registerToplevelFunction("move-backwards!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 2) { throw std::runtime_error("move-backwards! expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto distance = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                    if (distance) {
                        turtleDescriptor->moveBackwards(distance->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to move-backwards! must be a number.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to move-backwards! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("rotate-left!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 2) { throw std::runtime_error("rotate-left! expects 2 parameters."); }

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
                    throw std::runtime_error("First parameter to rotate-left! must be a turtle.");
                }

                return std::make_shared<NilValue>();
            });

        interpreter->interpreterState.registerToplevelFunction("rotate-right!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 2) { throw std::runtime_error("rotate-right! expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto angle = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                    if (angle) {
                        turtleDescriptor->rotateRight(angle->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to rotate-right! must be a number.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to rotate-right! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("pen-colour!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 2) { throw std::runtime_error("pen-colour! expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto colour = std::dynamic_pointer_cast<StringValue>(*parametersIterator);

                    if (colour) {
                        turtleDescriptor->penColour(colour->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to pen-colour! must be a colour name.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to pen-colour! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("pen-size!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 2) { throw std::runtime_error("pen-size! expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    auto size = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                    if (size) {
                        turtleDescriptor->penSize(size->value);
                    }
                    else {
                        throw std::runtime_error("Second parameter to pen-size! must be a number.");
                    }
                }
                else {
                    throw std::runtime_error("First parameter to pen-size! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("pen-on!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 1) { throw std::runtime_error("pen-on! expects 1 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    turtleDescriptor->penOn();
                }
                else {
                    throw std::runtime_error("First parameter to pen-on! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("pen-off!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 1) { throw std::runtime_error("pen-off! expects 1 parameters."); }

                auto parametersIterator = parameters.begin();

                auto turtleDescriptor = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
                parametersIterator++;

                if (turtleDescriptor) {
                    turtleDescriptor->penOff();
                }
                else {
                    throw std::runtime_error("First parameter to pen-off! must be a turtle.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerToplevelFunction("sleep!",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 1) { throw std::runtime_error("sleep! expects 1 parameter."); }

                auto sleepLength = std::dynamic_pointer_cast<NumberValue>(parameters.front());

                if (sleepLength) {
                    std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(sleepLength->value));
                }
                else {
                    throw std::runtime_error("Turtle must be bound to a name.");
                }

                return std::make_shared<NilValue>();
        });

        interpreter->interpreterState.registerMacro("while",
            [](InterpreterState &state, MacroParameters parameters) {
                auto whileCall = std::make_shared<Parser::AST::CallNode>(std::make_shared<Parser::AST::NameLiteralNode>("while*"),
                                                                         std::make_shared<std::vector<std::shared_ptr<Parser::AST::Node>>>());
                auto implicitDo = std::make_shared<Parser::AST::CallNode>(std::make_shared<Parser::AST::NameLiteralNode>("do"),
                                                                          std::make_shared<std::vector<std::shared_ptr<Parser::AST::Node>>>());

                auto lambdaName = state.gensym("lambda");
                auto lambdaNameValue = std::make_shared<Parser::AST::NameLiteralNode>(lambdaName);

                for (auto loopBodyForm : parameters) {
                    implicitDo->parameters->push_back(loopBodyForm);
                }

                auto functionValue = std::make_shared<UserDefinedFunctionValue>(std::list<std::string>(), implicitDo);

                state.assignToName(lambdaName, functionValue);

                whileCall->parameters->push_back(lambdaNameValue);

                return whileCall;
        });

        interpreter->interpreterState.registerToplevelFunction("while*",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                auto &state = visitor->interpreterState;
                if (parameters.size() != 1) { throw std::runtime_error("while-impl expects 1 parameter."); }

                auto loopBody = std::dynamic_pointer_cast<FunctionValue>(parameters.front());

                if (loopBody) {
                    while (true) {
                        loopBody->call(visitor);
                    }
                }
                else {
                    throw std::runtime_error("Loop body must be a function.");
                }

                return std::make_shared<NilValue>();
            });

        //interpreter->interpreterState.registerMacro("set!",
        //    [](InterpreterState &state, MacroParameters parameters) {
        //        if (parameters.size() != 2) { throw std::runtime_error("set! expects 2 parameters."); }

        //        auto parametersIterator = parameters.begin();

        //        auto implCall = std::make_shared<Parser::AST::CallNode>(std::make_shared<Parser::AST::NameLiteralNode>("set*"),
        //                                                                std::make_shared<std::vector<std::shared_ptr<Parser::AST::Node>>>());

        //        for (auto setParam : parameters) {
        //            implCall->parameters->push_back(setParam);
        //        }

        //        auto functionValue = std::make_shared<UserDefinedFunctionValue>(std::list<std::string>(), implicitDo);

        //        state.assignToName(lambdaName, functionValue);

        //        whileCall->parameters->push_back(lambdaNameValue);

        //        return whileCall;

        //        //auto variableName = std::dynamic_pointer_cast<TurtleValue>(*parametersIterator);
        //        //parametersIterator++;

        //        //if (turtleDescriptor) {
        //        //    auto angle = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

        //        //    if (angle) {
        //        //        turtleDescriptor->rotateRight(angle->value);
        //        //    }
        //        //    else {
        //        //        throw std::runtime_error("Second parameter to rotate-right! must be a number.");
        //        //    }
        //        //}
        //        //else {
        //        //    throw std::runtime_error("First parameter to rotate-right! must be a turtle.");
        //        //}

        //        // return std::make_shared<NilValue>();
        //    });

        interpreter->interpreterState.registerToplevelFunction("set*",
            [](InterpreterVisitor *visitor, FunctionParameters parameters) {
                if (parameters.size() != 2) { throw std::runtime_error("set-impl expects 2 parameters."); }

                auto parametersIterator = parameters.begin();

                auto variableName = std::dynamic_pointer_cast<StringValue>(*parametersIterator++);
                auto variableValue = *parametersIterator;

                visitor->interpreterState.assignToName(variableName->value, variableValue);

                // if (turtleDescriptor) {
                //    auto angle = std::dynamic_pointer_cast<NumberValue>(*parametersIterator);

                //    if (angle) {
                //        turtleDescriptor->rotateRight(angle->value);
                //    }
                //    else {
                //        throw std::runtime_error("Second parameter to rotate-right! must be a number.");
                //    }
                // }
                // else {
                //    throw std::runtime_error("First parameter to rotate-right! must be a turtle.");
                // }

                return variableValue;
            });

        // interpreter->interpreterState.push();

        return interpreter;
    }
}