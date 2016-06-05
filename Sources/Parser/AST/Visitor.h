#pragma once

#include "AST.h"

namespace Parser
{
    namespace AST
    {
        // class NameLiteralNode;
        // class StringLiteralNode;
        // class NumberLiteralNode;
        // class CallNode;

        class Node;

        class VoidVisitor
        {
            public:
                virtual void visit(Node *) = 0;
                // virtual void visit(NameLiteralNode *) = 0;
                // virtual void visit(StringLiteralNode *) = 0;
                // virtual void visit(NumberLiteralNode *) = 0;
                // virtual void visit(CallNode *) = 0;
        };
    }
}