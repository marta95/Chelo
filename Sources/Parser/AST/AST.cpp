#include "AST.h"

#include "PrettyPrintVisitor.h"

#include <sstream>

namespace Parser
{
    namespace AST
    {
        NameLiteralNode::NameLiteralNode(std::string name) :
            Node(), name(name)
        {

        }

        std::string NameLiteralNode::toString()
        {
            return name;
        }

        StringLiteralNode::StringLiteralNode(std::string content) :
            Node(), content(content)
        {

        }

        std::string StringLiteralNode::toString()
        {
            return content;
        }

        NumberLiteralNode::NumberLiteralNode(double value) :
            Node(), value(value)
        {

        }

        std::string NumberLiteralNode::toString()
        {
            return std::to_string(value);
        }

        CallNode::CallNode(std::shared_ptr<Node> functionName,
                           std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters) :
            Node(), functionName(functionName), parameters(parameters)
        {

        }

        std::string CallNode::toString()
        {
            std::stringstream stringRepresentation("");

            stringRepresentation << "(" << functionName->toString();

            for (auto parameter : *parameters) {
                stringRepresentation << std::endl << "  " << parameter->toString();
            }

            stringRepresentation << ")";

            return stringRepresentation.str();
        }
    }
}