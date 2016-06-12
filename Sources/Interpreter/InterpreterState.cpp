#include "InterpreterState.h"

namespace Interpreter
{
    InterpreterState &InterpreterState::registerToplevelFunction(std::string name, BuiltInFunction function)
    {
        // TODO add assignment to toplevel frame;
        auto functionValue = std::make_shared<BuiltInFunctionValue>(std::list<std::string>(), function);
        stack.assignToName(name, functionValue);
        return *this;
    }

    InterpreterState &InterpreterState::assignToName(std::string &name, std::shared_ptr<InterpreterValue> value)
    {
        stack.assignToName(name, value);

        return *this;
    }

    optional<std::shared_ptr<InterpreterValue>> InterpreterState::lookup(std::string &name)
    {
        return stack.lookup(name);
    }

    InterpreterState &InterpreterState::addTurtle(std::string name, std::shared_ptr<Turtle::Turtle> turtle)
    {
        {
            std::lock_guard<std::mutex> guard(turtleMutex);
            turtleMap[name] = turtle;
        }

        return *this;
    }
}