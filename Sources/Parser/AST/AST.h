#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Visitor.h"

namespace Parser
{
    namespace AST
    {
        struct Node : public Visitable<int, Node>
        {
            public:
                virtual std::string toString() = 0;
                virtual std::vector<std::shared_ptr<Node>> getChildren() = 0;
                // int acceptVisitor(VisitorBase &v) override
                // {
                //     // dupa(*this, v);
                //     return 12;
                // }

                // int acceptVisitor(VisitorBase &visitor)
                // {
                //     // throw std::runtime_error("sth wrong");
                //     return 12;
                // }                
        };

        struct NameLiteralNode : public Node, public Visitable<int, NameLiteralNode>
        {
                std::string name;

            // public:
                NameLiteralNode(std::string name);
                std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct StringLiteralNode : public Node, public Visitable<int, StringLiteralNode>
        {
                std::string content;

            // public:
                StringLiteralNode(std::string content);
                std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct NumberLiteralNode : public Node, public Visitable<int, NumberLiteralNode>
        {
                double value;

            // public:
                NumberLiteralNode(double number);
                std::string toString() override;
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct CallNode : public Node, public Visitable<int, CallNode>
        {
                std::shared_ptr<Node> functionName;
                std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return *parameters;
                }

            // public:
                CallNode(std::shared_ptr<Node> functionName,
                         std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters);
                std::string toString() override;
        };
    }
}