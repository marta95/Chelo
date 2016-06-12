#pragma once

#include <deque>
#include <memory>
#include <map>

#include "../Parser/AST/AST.h"
#include "InterpreterValue.h"
#include "../Utilities/optional.h"

namespace Interpreter
{
    using namespace std::experimental;

    class StackFrame
    {
            std::map<std::string, std::shared_ptr<InterpreterValue>> symbolTable;

        public:
            StackFrame &assignToName(std::string &name, std::shared_ptr<InterpreterValue> value);
            optional<std::shared_ptr<InterpreterValue>> lookup(std::string &name);
    };

    class Stack
    {
            std::deque<StackFrame> stack;

        public:
            Stack();
            Stack &enterFrame();
            Stack &exitFrame();
            Stack &assignToName(std::string &name, std::shared_ptr<InterpreterValue> value);
            optional<std::shared_ptr<InterpreterValue>> lookup(std::string &name);
    };
}