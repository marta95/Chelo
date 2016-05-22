#pragma once

#include <sstream>
#include <vector>
#include <iterator>
#include <functional>
#include <algorithm>
#include <memory>

#include "IVisitor.h"
#include "IVisitable.h"

namespace AST {
    std::string join(std::vector<std::string> vector, std::string separator="")
    {
        switch (vector.size())
        {
            case 0:
                return "";
            case 1:
                return vector[0];
            default:
                std::ostringstream os;
                std::copy(vector.begin(), vector.end()-1, std::ostream_iterator<std::string>(os, separator.c_str()));
                os << *vector.rbegin();
                return os.str();
        }
    }

    template<typename T, typename U>
    std::vector<T> mapDerp(std::vector<U> &in, std::function<T(U)> function)
    {
        std::vector<T> output(in.size());

        std::transform(in.begin(), in.end(), output.begin(), function);

        return output;
    };

    class Form : public IVisitable {
    protected:
        std::vector <std::shared_ptr<Form>> elements;

    public:
        Form() :
            elements({})
        {

        }

        void addElement(std::shared_ptr<Form> element)
        {
            elements.push_back(element);
        }

        std::vector<std::shared_ptr<Form>> &getElements()
        {
            return elements;
        }

        virtual std::string to_str() = 0;
    };

    class List : public Form {
    public:
        std::string to_str()
        {
           return "(" + join(mapDerp<std::string, std::shared_ptr<Form>>(elements, [](auto form) {
                               return form->to_str();
                             }), " ") + ")";
        }

        std::shared_ptr<Value> acceptVisitor(IVisitor &visitor)
        {
            return visitor.visit(*this);
        }
    };

    class NameLiteral : public Form {
    public:
        const std::string &getName() const {
            return name;
        }

        void setName(const std::string &name) {
            NameLiteral::name = name;
        }

    private:
        std::string name;
    public:
        std::string to_str()
        {
            return name;
        }

        std::shared_ptr<Value> acceptVisitor(IVisitor &visitor)
        {
            return visitor.visit(*this);
        }
    };

    class Vector : public Form {
    public:
        std::string to_str()
        {
            return "[" + join(mapDerp<std::string, std::shared_ptr<Form>>(elements, [](auto form) {
                return form->to_str();
            }), " ") + "]";
        }

        std::shared_ptr<Value> acceptVisitor(IVisitor &visitor)
        {
            return visitor.visit(*this);
        }
    };

    class StringLiteral : public Form {
    public:
        const std::string &getText() const {
            return text;
        }

        void setText(const std::string &text) {
            StringLiteral::text = text;
        }

    private:
        std::string text;
    public:
        std::string to_str()
        {
            return "\"" + text + "\"";
        }

        std::shared_ptr<Value> acceptVisitor(IVisitor &visitor)
        {
            return visitor.visit(*this);
        }
    };

    class IntegerLiteral : public Form {
    public:
        int getValue() const {
            return value;
        }

        void setValue(int value) {
            IntegerLiteral::value = value;
        }

        std::shared_ptr<Value> acceptVisitor(IVisitor &visitor)
        {
            return visitor.visit(*this);
        }

    private:
        int value;
    public:
        std::string to_str()
        {
            return std::to_string(value);
        }
    };
}