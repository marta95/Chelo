#pragma once

#include <vector>
#include <memory>
#include <string>

namespace Parser
{
    namespace AST
    {
        class Node
        {
            public:
                virtual std::string toString() = 0;
        };

        struct NameLiteralNode : public Node
        {
                std::string name;

            // public:
                NameLiteralNode(std::string name);
                std::string toString();
        };

        struct CallNode : public Node
        {
                std::shared_ptr<Node> functionName;
                std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters;

            // public:
                CallNode(std::shared_ptr<Node>                               functionName,
                         std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters);
                std::string toString();
        };
    }
}