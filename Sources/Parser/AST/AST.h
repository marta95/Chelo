#pragma once

#include <vector>
#include <memory>
#include <string>

#include "Visitor.h"

namespace Parser
{
    namespace AST
    {
        struct Node : public Visitable<int>
        {
            public:
                virtual std::string toString() = 0;
                virtual std::vector<std::shared_ptr<Node>> getChildren() = 0;
                int acceptVisitor(VisitorBase &v) override
                {
                    // dupa(*this, v);
                }
        };

        struct NameLiteralNode : public Node //, public Visitable<NameLiteralNode>
        {
                std::string name;

            // public:
                NameLiteralNode(std::string name);
                std::string toString();
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct StringLiteralNode : public Node //, public Visitable<NameLiteralNode>
        {
                std::string content;

            // public:
                StringLiteralNode(std::string content);
                std::string toString();
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct NumberLiteralNode : public Node //, public Visitable<NameLiteralNode>
        {
                double value;

            // public:
                NumberLiteralNode(double number);
                std::string toString();
                std::vector<std::shared_ptr<Node>> getChildren() override
                {
                    return std::vector<std::shared_ptr<Node>>();
                }
        };

        struct CallNode : public Node // , public Visitable<NameLiteralNode>
        {
                std::shared_ptr<Node> functionName;
                std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters;
                std::vector<std::shared_ptr<Node>> getChildren() override 
                {
                    return *parameters;
                }

            // public:
                CallNode(std::shared_ptr<Node>                               functionName,
                         std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters);
                std::string toString();
        };
    }
}