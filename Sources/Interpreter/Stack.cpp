#include "Stack.h"

#include "../Utilities/ReverseContainer.h"

namespace Interpreter
{
    StackFrame &StackFrame::assignToName(std::string &name, std::shared_ptr<InterpreterValue> value)
    {
        symbolTable[name] = value;
        return *this;
    }

    optional<std::shared_ptr<InterpreterValue>> StackFrame::lookup(std::string &name)
    {
        auto iterator = symbolTable.find(name);

        if (iterator != symbolTable.end()) {
            return make_optional(iterator->second);
        }
        else {
            return nullopt;
        }
    }

    Stack::Stack() :
        stack()
    {
        stack.push_back(StackFrame()); // top level frame
        stack.push_back(StackFrame()); // first stack frame
    }

    Stack &Stack::enterFrame()
    {
        stack.push_back(StackFrame());
        return *this;
    }

    Stack &Stack::exitFrame()
    {
        // TODO sanity check for at least one frame
        stack.pop_back();
        return *this;
    }

    Stack &Stack::assignToToplevelName(std::string &name, std::shared_ptr<InterpreterValue> value)
    {
        stack.front().assignToName(name, value);
        return *this;
    }

    Stack &Stack::assignToName(std::string &name, std::shared_ptr<InterpreterValue> value)
    {
        stack.back().assignToName(name, value);
        return *this;
    }

    optional<std::shared_ptr<InterpreterValue>> Stack::lookup(std::string &name)
    {
        for (auto frame : reverse(stack)) {
            auto optionalValue = frame.lookup(name);

            if (optionalValue) return optionalValue.value();
        }

        return nullopt;
    }

}