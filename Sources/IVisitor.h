#pragma once

//class IVisitable;

#include <memory>

namespace AST {
    class List;
    class NameLiteral;
    class Vector;
    class StringLiteral;
    class IntegerLiteral;
}

class Value
{
    public:
        virtual std::string toString() const = 0;
};

class NilValue : public Value
{
    public:
        std::string toString() const
        {
            return "nil";
        }
};

class StringValue : public Value
{
    private:
        std::string value;

    public:
        const std::string &getValue() const {
            return value;
        }

        void setValue(const std::string &value) {
            StringValue::value = value;
        }

        std::string toString() const {
            return value;
        }
};

class IntegerValue : public Value
{
private:
    int value;

public:
    const int getValue() const {
        return value;
    }

    void setValue(const int value) {
        IntegerValue::value = value;
    }

    std::string toString() const {
        return std::to_string(value);
    }
};


class IVisitor {
    public:
        virtual std::shared_ptr<Value> visit(AST::List &) = 0;
        virtual std::shared_ptr<Value> visit(AST::NameLiteral &) = 0;
        virtual std::shared_ptr<Value> visit(AST::Vector &) = 0;
        virtual std::shared_ptr<Value> visit(AST::StringLiteral &) = 0;
        virtual std::shared_ptr<Value> visit(AST::IntegerLiteral &) = 0;
};
