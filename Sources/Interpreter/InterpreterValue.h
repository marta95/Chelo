#pragma once

#include <list>
#include <string>
#include <sstream>
#include <functional>

#include "../Parser/AST/AST.h"
#include "../Turtle/Turtle.h"

#include "InterpreterVisitor.h"

namespace Interpreter
{
    class InterpreterState;

    typedef std::shared_ptr<InterpreterValue> FunctionResult;
    typedef std::list<std::shared_ptr<InterpreterValue>> FunctionParameters;
    typedef std::function<FunctionResult(InterpreterVisitor *, FunctionParameters)> BuiltInFunction;

    typedef std::shared_ptr<Parser::AST::Node> MacroResult;
    typedef std::list<std::shared_ptr<Parser::AST::Node>> MacroParameters;
    typedef std::function<MacroResult(InterpreterState &, MacroParameters)> BuiltInMacro;

    struct InterpreterValue
    {
        public:
            virtual std::string stringRepr() = 0;
    };

    struct NilValue : public InterpreterValue
    {
        public:
            std::string stringRepr();
    };

    struct FunctionValue : public InterpreterValue
    {
        public:
            FunctionResult call(InterpreterVisitor *visitor);
            virtual FunctionResult call(InterpreterVisitor *visitor, FunctionParameters params) = 0;
    };

    struct UserDefinedFunctionValue : public FunctionValue
    {
            std::list<std::string> paramNames;
            std::shared_ptr<Parser::AST::Node> expression;

        public:
            UserDefinedFunctionValue(std::list<std::string> paramNames,
                                     std::shared_ptr<Parser::AST::Node> expression);
            std::string stringRepr();

            FunctionResult call(InterpreterVisitor *visitor, FunctionParameters params);
    };

    struct BuiltInFunctionValue : public FunctionValue
    {
            std::list<std::string> paramNames;
            BuiltInFunction function;

        public:
            BuiltInFunctionValue(std::list<std::string> paramNames,
                                 BuiltInFunction function);
            std::string stringRepr();
            FunctionResult call(InterpreterVisitor *visitor, FunctionParameters params);
    };

    struct BuiltInMacroValue : public InterpreterValue
    {
        std::list<std::string> paramNames;
        BuiltInMacro function;

    public:
        BuiltInMacroValue(std::list<std::string> paramNames,
                          BuiltInMacro function);

        std::string stringRepr();

        MacroResult call(InterpreterState &state, MacroParameters params);
    };

    struct StringValue : public InterpreterValue
    {
            std::string value;

        public:
            StringValue(std::string value);

            std::string stringRepr();
    };

    struct NumberValue : public InterpreterValue
    {
            double value;

        public:
            NumberValue(double value);

            std::string stringRepr();
    };

    struct VectorValue : public InterpreterValue
    {
            std::list<std::shared_ptr<InterpreterValue>> vector;
        public:
            VectorValue(std::list<std::shared_ptr<InterpreterValue>> vector);

            std::string stringRepr();
    };

    struct TurtleValue : public InterpreterValue
    {
            std::shared_ptr<Turtle::Turtle > turtle;
        public:
            TurtleValue(std::shared_ptr<Turtle::Turtle> turtle);

            TurtleValue &moveForward(double lineLength)   ;
            TurtleValue &moveBackwards(double lineLength) ;
            TurtleValue &rotateLeft(double degrees)       ;
            TurtleValue &rotateRight(double degrees)      ;
            TurtleValue &penOn()                          ;
            TurtleValue &penOff()                         ;
            TurtleValue &penColour(std::string newColour) ;
            TurtleValue &penSize(double pixelSize)        ;

            std::string stringRepr();
    };
}