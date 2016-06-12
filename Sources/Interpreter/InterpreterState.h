#pragma once

#include <list>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <mutex>

#include "Stack.h"
#include "InterpreterValue.h"
#include "../Turtle/Turtle.h"

namespace Interpreter
{
    typedef std::map<std::string, std::shared_ptr<Turtle::Turtle>> TurtleMap;

    class InterpreterState
    {
            std::mutex turtleMutex;
            TurtleMap turtleMap;
            Stack stack;
            std::istream &inputStream;
            std::ostream &outputStream;

        public:
            InterpreterState() :
                stack(), inputStream(std::cin), outputStream(std::cout)
            {

            }

            std::istream &getInputStream()
            {
                return inputStream; 
            }

            std::ostream &getOutputStream()
            {
                return outputStream; 
            }

            TurtleMap getTurtles()
            {
                {
                    std::lock_guard<std::mutex> guard(turtleMutex);

                    return turtleMap;
                }
            }

            InterpreterState &registerToplevelFunction(std::string name, BuiltInFunction function);

            /*InterpreterState &enterFrame();
            InterpreterState &exitFrame();*/

            InterpreterState &assignToName(std::string &name, std::shared_ptr<InterpreterValue> value);
            optional<std::shared_ptr<InterpreterValue>> lookup(std::string &name);
            InterpreterState &addTurtle(std::string name, std::shared_ptr<Turtle::Turtle> turtle);
    };
}