#include "AST.h"

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

        CallNode::CallNode(std::shared_ptr<Node> functionName,
                           std::shared_ptr<std::vector<std::shared_ptr<Node>>> parameters) :
            Node(), functionName(functionName), parameters(parameters)
        {

        }

        std::string CallNode::toString()
        {
            std::stringstream stringRepresentation("");

            stringRepresentation << "(" << functionName->toString();
            
            if (parameters->size() > 0) {
                stringRepresentation << std::endl;
            }

            for (auto parameter : *parameters) {
                stringRepresentation << "  " << parameter->toString() << std::endl;
            }

            stringRepresentation << ")";

            return stringRepresentation.str();
        }
    }
}