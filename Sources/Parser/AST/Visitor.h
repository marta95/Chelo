#pragma once

#include "AST.h"

namespace Parser {
    namespace AST {
        class VisitorBase
        {
            public:
                virtual ~VisitorBase()
                {

                }
        };

        template<typename ResultT, typename VisitableT>
        class Visitor
        {
            public:
                virtual ResultT visit(VisitableT *) = 0;
        };

        template <typename ResultT>
        class Visitable
        {
            public:
                // typedef Re
                virtual ~Visitable()
                {

                }

                ResultT acceptVisitor(VisitorBase *visitor)
                {
                    throw std::runtime_error("sth wrong");
                }

            protected:
                template <class VisitedT>
                ResultT dupa(VisitedT &visited, VisitorBase visitor)
                {
                    if (Visitor<ResultT, VisitedT> *p = dynamic_cast<Visitor<ResultT, VisitedT> *> (visitor))
                    {
                        return p->visit(visited);
                    }
                    throw std::runtime_error("sth wrong");
                }
        };
    }
}