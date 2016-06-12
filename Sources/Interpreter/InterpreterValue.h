#pragma once

#include <list>
#include <string>
#include <sstream>
#include <functional>

#include "../Parser/AST/AST.h"
#include "../Turtle/Turtle.h"

namespace Interpreter
{
    class InterpreterState;

    typedef std::shared_ptr<InterpreterValue> FunctionResult;
    typedef std::list<std::shared_ptr<InterpreterValue>> FunctionParameters;
    typedef std::function<FunctionResult (InterpreterState &, FunctionParameters)> BuiltInFunction;

    struct InterpreterValue
    {
        public:
            virtual std::string stringRepr() = 0;
    };

    struct NilValue : public InterpreterValue
    {
        public:
            std::string stringRepr() { return "nil";  }
    };

    struct FunctionValue : public InterpreterValue
    {
        public:
            virtual std::shared_ptr<InterpreterValue> call(InterpreterState &state, FunctionParameters params) = 0;
    };

    struct UserDefinedFunctionValue : public FunctionValue
    {
            std::list<std::string> paramNames;
            std::shared_ptr<Parser::AST::Node> expression;

        public:
            UserDefinedFunctionValue(std::list<std::string> paramNames,
                                     std::shared_ptr<Parser::AST::Node> expression) :
                paramNames(paramNames), expression(expression)
            {

            }

            std::string stringRepr()
            {
                return "(fn ...)";
            }

            std::shared_ptr<InterpreterValue> call(InterpreterState &state, FunctionParameters params)
            {
                throw std::runtime_error("NOT IMPLEMENTED YET");
            }
    };

    struct BuiltInFunctionValue : public FunctionValue
    {
            std::list<std::string> paramNames;
            BuiltInFunction function;

        public:
            BuiltInFunctionValue(std::list<std::string> paramNames,
                                 BuiltInFunction function) :
                paramNames(paramNames), function(function)
            {

            }

            std::string stringRepr()
            {
                return "(#native ...)";
            }

            std::shared_ptr<InterpreterValue> call(InterpreterState &state, FunctionParameters params)
            {
                return function(state, params);
            }
    };

    struct StringValue : public InterpreterValue
    {
            std::string value;

        public:
            StringValue(std::string value) :
                value(value)
            {

            }

            std::string stringRepr()
            {
                return value;
            }
    };

    struct NumberValue : public InterpreterValue
    {
            double value;

        public:
            NumberValue(double value) :
                value(value)
            {

            }

            std::string stringRepr()
            {
                return std::to_string(value);
            }
    };

    struct VectorValue : public InterpreterValue
    {
            std::list<std::shared_ptr<InterpreterValue>> vector;
        public:
            VectorValue(std::list<std::shared_ptr<InterpreterValue>> vector) :
                vector(vector)
            {

            }

            std::string stringRepr()
            {
                auto stringStream = std::stringstream("");

                stringStream << "[";

                for (auto value : vector) {
                    stringStream << value->stringRepr() << " ";
                }

                stringStream << "]";

                return stringStream.str();
            }
    };

    struct TurtleValue : public InterpreterValue
    {
            std::shared_ptr<Turtle::Turtle > turtle;
        public:
            TurtleValue(std::shared_ptr<Turtle::Turtle> turtle) :
                turtle(turtle)
            {

            }

            TurtleValue &moveForward(double lineLength)   { turtle->moveForward(lineLength); return *this; }
            TurtleValue &moveBackwards(double lineLength) { turtle->moveBackwards(lineLength); return *this; }
            TurtleValue &rotateLeft(double degrees)       { turtle->rotateLeft(degrees); return *this; }
            TurtleValue &rotateRight(double degrees)      { turtle->rotateRight(degrees); return *this; }
            TurtleValue &penOn()                          { turtle->penOn(); return *this; }
            TurtleValue &penOff()                         { turtle->penOff(); return *this; }
            TurtleValue &penColour(std::string newColour) { turtle->penColour(newColour); return *this; }
            TurtleValue &penSize(double pixelSize)        { turtle->penSize(pixelSize); return *this; }

            std::string stringRepr() { return "turtle"; }
    };
}