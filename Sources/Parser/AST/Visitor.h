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
                virtual ResultT visit(VisitableT &) = 0;
        };

        template <typename ResultT>
        class Visitable
        {
            public:
                // typedef Re
                virtual ~Visitable()
                {

                }

                ResultT acceptVisitor(::Parser::AST::VisitorBase &visitor)
                {
                    throw std::runtime_error("sth wrong");
                }

            protected:
                template <class VisitedT>
                ResultT dupa(VisitedT &visited, ::Parser::AST::VisitorBase &visitor)
                {
                    if (Visitor<ResultT, VisitedT> *p = dynamic_cast<Visitor<ResultT, VisitedT> *> (&visitor))
                    {
                        return p->visit(visited);
                    }
                    throw std::runtime_error("sth wrong");
                }
        };


        // template<typename T>
        // class IVisitor {
        //     public:
        //         virtual T visit(NameLiteralNode &) =0;
        //         virtual T visit(StringLiteralNode &) =0;
        //         virtual T visit(NumberLiteralNode &) =0;
        //         virtual T visit(CallNode &) =0;
        // };

        // template<typename T>
        // class IVisitable {
        //     public:
        //         virtual T acceptVisitor(IVisitor<T> &) = 0;
        // };
    }
}