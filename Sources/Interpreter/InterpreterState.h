#pragma once

#include <list>
#include <functional>
#include <iostream>
#include <memory>
#include <map>
#include <mutex>

#include "Stack.h"
#include "../Turtle/Turtle.h"
#include "InterpreterValue.h"

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
            unsigned int gensymCounter = 0;

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

            std::string gensym(std::string prefix = "")
            {
                if (prefix.size() > 0) {
                    return prefix + "-" + std::to_string(gensymCounter++);
                }
                else {
                    return "gensym-" + std::to_string(gensymCounter++);
                }
            }

            InterpreterState &registerToplevelFunction(std::string name, BuiltInFunction function);
            InterpreterState &InterpreterState::registerMacro(std::string name, BuiltInMacro function);

            /*InterpreterState &enterFrame();
            InterpreterState &exitFrame();*/

            InterpreterState &assignToName(std::string &name, std::shared_ptr<InterpreterValue> value);
            optional<std::shared_ptr<InterpreterValue>> lookup(std::string &name);
            InterpreterState &addTurtle(std::string name, std::shared_ptr<Turtle::Turtle> turtle);
    };
}