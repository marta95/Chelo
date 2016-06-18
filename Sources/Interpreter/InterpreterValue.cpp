#pragma once

#include <list>
#include <string>
#include <sstream>
#include <functional>

#include "../Parser/AST/AST.h"
#include "../Turtle/Turtle.h"

#include "InterpreterValue.h"
#include "InterpreterVisitor.h"

namespace Interpreter
{
    // class InterpreterState;

    std::string NilValue::stringRepr() { return "nil";  }

    FunctionResult FunctionValue::call(InterpreterVisitor *visitor)
    {
        return call(visitor, std::list<std::shared_ptr<InterpreterValue>>());
    }

    UserDefinedFunctionValue::UserDefinedFunctionValue(std::list<std::string> paramNames,
                             std::shared_ptr<Parser::AST::Node> expression) :
        paramNames(paramNames), expression(expression)
    {

    }

    std::string UserDefinedFunctionValue::stringRepr()
    {
        return "(fn ...)";
    }

    FunctionResult UserDefinedFunctionValue::call(InterpreterVisitor *visitor, FunctionParameters params)
    {
        auto castExpression = std::dynamic_pointer_cast<Parser::AST::CallNode>(expression);

        // throw std::runtime_error("NOT IMPLEMENTED YET");

        return expression->acceptVisitor(visitor);
    }

    BuiltInFunctionValue::BuiltInFunctionValue(std::list<std::string> paramNames,
                         BuiltInFunction function) :
        paramNames(paramNames), function(function)
    {

    }

    std::string BuiltInFunctionValue::stringRepr()
    {
        return "(#native ...)";
    }

    FunctionResult BuiltInFunctionValue::call(InterpreterVisitor *visitor, FunctionParameters params)
    {
        return function(visitor, params);
    }

    BuiltInMacroValue::BuiltInMacroValue(std::list<std::string> paramNames,
                      BuiltInMacro function) :
        paramNames(paramNames), function(function)
    {

    }

    std::string BuiltInMacroValue::stringRepr()
    {
        return "(#macro ...)";
    }

    MacroResult BuiltInMacroValue::call(InterpreterState &state, MacroParameters params)
    {
        return function(state, params);
    }

    StringValue::StringValue(std::string value) :
        value(value)
    {

    }

    std::string StringValue::stringRepr()
    {
        return value;
    }

    NumberValue::NumberValue(double value) :
        value(value)
    {

    }

    std::string NumberValue::stringRepr()
    {
        return std::to_string(value);
    }

    VectorValue::VectorValue(std::list<std::shared_ptr<InterpreterValue>> vector) :
        vector(vector)
    {

    }

    std::string VectorValue::stringRepr()
    {
        auto stringStream = std::stringstream("");

        stringStream << "[";

        for (auto value : vector) {
            stringStream << value->stringRepr() << " ";
        }

        stringStream << "]";

        return stringStream.str();
    }
    
    TurtleValue::TurtleValue(std::shared_ptr<Turtle::Turtle> turtle) :
        turtle(turtle)
    {

    }

    TurtleValue &TurtleValue::moveForward(double lineLength)   { turtle->moveForward(lineLength); return *this; }
    TurtleValue &TurtleValue::moveBackwards(double lineLength) { turtle->moveBackwards(lineLength); return *this; }
    TurtleValue &TurtleValue::rotateLeft(double degrees)       { turtle->rotateLeft(degrees); return *this; }
    TurtleValue &TurtleValue::rotateRight(double degrees)      { turtle->rotateRight(degrees); return *this; }
    TurtleValue &TurtleValue::penOn()                          { turtle->penOn(); return *this; }
    TurtleValue &TurtleValue::penOff()                         { turtle->penOff(); return *this; }
    TurtleValue &TurtleValue::penColour(std::string newColour) { turtle->penColour(newColour); return *this; }
    TurtleValue &TurtleValue::penSize(double pixelSize)        { turtle->penSize(pixelSize); return *this; }

    std::string TurtleValue::stringRepr() { return "turtle"; }
}