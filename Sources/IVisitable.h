#pragma once

#include <memory>

class IVisitor;
class Value;

class IVisitable {
    public:
        virtual std::shared_ptr<Value> acceptVisitor(IVisitor &) = 0;
};
