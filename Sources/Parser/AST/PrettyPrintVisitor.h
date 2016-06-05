#pragma once

#include <iostream>
#include <sstream>

#include "Visitor.h"
#include "AST.h"

namespace Parser {
    namespace AST {
        class PrettyPrintVisitor : public VoidVisitor
        {
                int depth;
                std::string prefix = "  ";

                std::string pad();

            public:
                PrettyPrintVisitor();

                void visit(Node *);

                void __visit(NameLiteralNode *nameLiteral);
                void __visit(StringLiteralNode *stringLiteral);
                void __visit(NumberLiteralNode *numberLiteral);
                void __visit(CallNode *call);
        };
    }
}