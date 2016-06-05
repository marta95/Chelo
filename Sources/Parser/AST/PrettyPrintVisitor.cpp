#include "AST.h"
#include "PrettyPrintVisitor.h"

namespace Parser {
    namespace AST {

        std::string PrettyPrintVisitor::pad()
        {
            std::stringstream padString("");

            for (int i = 0; i < depth; i++) {
                padString << prefix;
            }

            return padString.str();
        }

        PrettyPrintVisitor::PrettyPrintVisitor()
        {

        }

        void PrettyPrintVisitor::visit(Node *whatever)
        {
            if (dynamic_cast<NameLiteralNode *>(whatever)) {
                NameLiteralNode *derp = dynamic_cast<NameLiteralNode *>(whatever);

                this->__visit(derp);
            }
            else if (dynamic_cast<StringLiteralNode *>(whatever)) {
                StringLiteralNode *derp = dynamic_cast<StringLiteralNode *>(whatever);

                this->__visit(derp);
            }
            else if (dynamic_cast<NumberLiteralNode *>(whatever)) {
                NumberLiteralNode *derp = dynamic_cast<NumberLiteralNode *>(whatever);

                this->__visit(derp);
            }
            else if (dynamic_cast<CallNode *>(whatever)) {
                CallNode *derp = dynamic_cast<CallNode *>(whatever);

                this->__visit(derp);
            }
            else {
                throw std::runtime_error("ugh");
            }
        }

        void PrettyPrintVisitor::__visit(NameLiteralNode *nameLiteral)
        {
            std::cout << nameLiteral->name;
        }

        void PrettyPrintVisitor::__visit(StringLiteralNode *stringLiteral)
        {
            std::cout << "\"" << stringLiteral->content << "\"";
        }

        void PrettyPrintVisitor::__visit(NumberLiteralNode *numberLiteral)
        {
            std::cout << std::to_string(numberLiteral->value);
        }

        void PrettyPrintVisitor::__visit(CallNode *call)
        {
            std::cout << pad() << "(";
        
            call->functionName->acceptVisitor(this);

            depth++;

            for (auto parameter : call->getChildren()) {
                std::cout << std::endl << pad();
                parameter->acceptVisitor(this);
            }

            depth--;

            std::cout << ")";
        }
    }
}