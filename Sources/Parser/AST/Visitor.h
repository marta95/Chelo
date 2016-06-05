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

        template <typename ResultT, typename VisitableT> class Visitor;

        template <typename ResultT, typename VisitableT>
        class Visitable
        {
            public:
                virtual ~Visitable() {};
                virtual ResultT acceptVisitor(VisitorBase &visitor)
                {
                    VisitableT *castThis = static_cast<VisitableT *>(this);
                    return __acceptVisitor(*castThis, visitor);
                }

            protected:
                static ResultT __acceptVisitor(VisitableT &visitable, VisitorBase &visitor)
                {
                    auto castVisitor = dynamic_cast<Visitor<ResultT, VisitableT> *>(&visitor);
                    if (castVisitor) {
                        return castVisitor->visit(visitable);
                    }
                    throw std::runtime_error("sth wrong");
                }
        };

        template <typename ResultT, typename VisitableT>
        class Visitor // : virtual public VisitorBase
        {
            public:
                virtual ResultT visit(VisitableT &visitable) = 0;
        };
    }
}