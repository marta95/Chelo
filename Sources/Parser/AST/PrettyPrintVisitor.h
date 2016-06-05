#pragma once

#include <iostream>
#include <sstream>

#include "Visitor.h"
#include "AST.h"

namespace Parser {
    namespace AST {
        class PrettyPrintVisitor : public VisitorBase,
                                   public Visitor<int, NameLiteralNode>,
                                   public Visitor<int, StringLiteralNode>,
                                   public Visitor<int, NumberLiteralNode>,
                                   public Visitor<int, CallNode>
        {
                int depth;
                std::string prefix = "  ";

                std::string pad()
                {
                    std::stringstream padString;

                    for (int i = 0; i < depth; i++) {
                        padString << prefix;
                    }

                    return padString.str();
                }

            public:
                PrettyPrintVisitor()
                {

                }

                int visit(NameLiteralNode &nameLiteral) override
                {
                    std::cout << nameLiteral.name;
                }

                int visit(StringLiteralNode &stringLiteral) override
                {
                    std::cout << "\"" << stringLiteral.content << "\"";
                }

                int visit(NumberLiteralNode &numberLiteral) override
                {
                    std::cout << std::to_string(numberLiteral.value);
                }

                int visit(CallNode &call) override
                {
                    std::cout << pad() << "(";
                
                    call.functionName->acceptVisitor(*this);

                    depth++;

                    for (auto parameter : call.getChildren()) {
                        std::cout << std::endl << pad();
                        parameter->acceptVisitor(*this);
                    }

                    depth--;

                    std::cout << ")";
                }
        };
    }
}