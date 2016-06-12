#pragma once

#include <memory>

#include "../Parser/AST/AST.h"
#include "InterpreterState.h"
#include "InterpreterVisitor.h"

namespace Interpreter
{
    class Interpreter
    {
            InterpreterState interpreterState;

            // Interpreter();
        public:
            void interpret(std::shared_ptr<Parser::AST::Node> expression);
            TurtleMap getTurtles() { return interpreterState.getTurtles(); }

            static std::shared_ptr<Interpreter> makeInterpreter();
    };
}