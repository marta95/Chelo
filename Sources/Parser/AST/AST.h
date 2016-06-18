#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Visitor.h"
// #include "../../Interpreter/InterpreterValue.h"

namespace Interpreter
{
    struct InterpreterValue;
}

namespace Parser
{
    namespace AST
    {
        struct Node: public Visitable<void> // : public VoidVisitable<PrettyPrintVisitor>
        {
            public:
                // virtual std::string toString() = 0;
                virtual std::vector<std::shared_ptr<Node>> getChildren() = 0;
                virtual void acceptVisitor(Visitor<void, Node> *visitor)
                {
                    visitor->visit(this);
                }
                virtual std::shared_ptr<Interpreter::InterpreterValue> acceptVisitor(Visitor<std::shared_ptr<Interpreter::InterpreterValue>, Node> *visitor)
                {
                    return visitor->visit(this);
                }
        };

        struct NameLiteralNode : public Node // , public Visitable<int, NameLiteralNode>
        {
                std::string name;

            // public:
                NameLiteralNode(std::string name);
                // std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct StringLiteralNode : public Node // , public Visitable<int, StringLiteralNode>
        {
                std::string content;

            // public:
                StringLiteralNode(std::string content);
                // std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct NumberLiteralNode : public Node // , public Visitable<int, NumberLiteralNode>
        {
                double value;

            // public:
                NumberLiteralNode(double number);
                // std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct CallNode : public Node // , public Visitable<int, CallNode>
        {
                std::shared_ptr<Node> functionName;
                std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return *parameters;
                }

            // public:
                CallNode(std::shared_ptr<Node> functionName) :
                    CallNode(functionName, std::make_shared<std::vector<std::shared_ptr<Parser::AST::Node>>>())
                {

                }
                CallNode(std::shared_ptr<Node> functionName,
                         std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters);
                // std::string toString() override;
        };

        struct VectorNode : public Node // , public Visitable<int, CallNode>
        {
                std::shared_ptr<std::vector<std::shared_ptr<Node>>> elements;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return *elements;
                }

            // public:
                VectorNode(std::shared_ptr<std::vector<std::shared_ptr<Node>>> elements);
                // std::string toString() override;
        };
    }
}