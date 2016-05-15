#pragma once

#include <memory>
#include <regex>
#include <iostream>
#include <deque>

#include "Source.h.h"
#include "AST.h"

class Program
{
	std::vector<std::shared_ptr<AST::Form>> instructionList;

	public:
		Program() :
			instructionList()
		{

		}

		Program &addForm(std::shared_ptr<AST::Form> &form)
		{
			instructionList.push_back(form);
			return *this;
		}
};

enum class ParserState {
	                     IN_WHITESPACE,
	                     IN_NAME_LITERAL,
                         IN_INTEGER_LITERAL,
						 IN_STRING_LITERAL
                       };
class Parser
{
	std::regex const MATCH_WHITESPACE{R"(\s+)"},
					 MATCH_OPEN_PARENTHESIS{R"(\()"},
					 MATCH_CLOSE_PARENTHESIS{R"(\))"},
					 MATCH_OPEN_LIST{R"(\[)"},
					 MATCH_CLOSE_LIST{R"(\])"},
					 MATCH_NAME_FIRST_CHARACTER{R"([a-zA-Z])"},
					 MATCH_NAME_REST_CHARACTER{R"([a-zA-Z0-9\-])"},
					 MATCH_INTEGER_LITERAL_CHARACTER{R"(\d)"},
					 MATCH_STRING_LITERAL_QUOTE{R"(")"},
                     MATCH_LINE_END{R"(\n)"};

	std::shared_ptr<Source> source;
	std::shared_ptr<Program> program;
	ParserState state;
    std::string accumulator;
    unsigned int column, line;
    std::deque<std::shared_ptr<AST::Form>> formStack;

    void add_form_element(std::shared_ptr<AST::Form> form)
    {
//        if (formStack.empty()) {
//            throw std::runtime_error(std::to_string(column) + ":" + std::to_string(line) + " top level literal not allowed");
//        }
        formStack.back()->addElement(form);
    }

    void unnest()
    {
//        if (formStack.size() <= 1) {
//            throw std::runtime_error(std::to_string(column) + ":" + std::to_string(line) + " top level literal not allowed");
//        }
        auto top = formStack.back();
        formStack.pop_back();
        formStack.back()->addElement(top);
        std::cout << "unnest stuff " << top->to_str() << std::endl;
    }

    std::shared_ptr<AST::Form> parse_whitespace(std::string &character)
    {
        std::cout << "IN_WHITESPACE" << std::endl;

        if (std::regex_match(character, MATCH_WHITESPACE)) {
            std::cout << "Next state: IN_WHITESPACE" << std::endl;
        } else if (std::regex_match(character, MATCH_OPEN_PARENTHESIS)) {
            formStack.push_back(std::make_shared<AST::List>());
            std::cout << "nest stuff" << std::endl;
        } else if (std::regex_match(character, MATCH_CLOSE_PARENTHESIS)) {
//            auto top = formStack.back();
//            formStack.pop_back();
//            std::cout << "unnest stuff: " << top->to_str() << std::endl;
            unnest();
        } else if (std::regex_match(character, MATCH_OPEN_LIST)) {
            formStack.push_back(std::make_shared<AST::Vector>());
            std::cout << "nest stuff" << std::endl;
        } else if (std::regex_match(character, MATCH_CLOSE_LIST)) {
//            auto top = formStack.back();
//            formStack.pop_back();
//            std::cout << "unnest stuff " << top->to_str() << std::endl;
            unnest();
        } else if (std::regex_match(character, MATCH_NAME_FIRST_CHARACTER)) {
            state = ParserState::IN_NAME_LITERAL;
            accumulator = character;
            std::cout << "start collecting name" << std::endl;
        } else if (std::regex_match(character, MATCH_INTEGER_LITERAL_CHARACTER)) {
            state = ParserState::IN_INTEGER_LITERAL;
            accumulator = character;
            std::cout << "start collecting integer" << std::endl;
        } else if (std::regex_match(character, MATCH_STRING_LITERAL_QUOTE)) {
            accumulator = "";
            state = ParserState::IN_STRING_LITERAL;
            std::cout << "start collecting string" << std::endl;
        } else {
            throw std::runtime_error(std::to_string(column) + ":" + std::to_string(line) + " unexpected stuff");
        }

        return std::shared_ptr<AST::Form>();
    }

    std::shared_ptr<AST::Form> parse_name(std::string &character)
    {
        std::cout << "IN_NAME_LITERAL" << std::endl;

        if (std::regex_match(character, MATCH_NAME_REST_CHARACTER)) {
            accumulator += character;
        } else if (std::regex_match(character, MATCH_WHITESPACE)) {
            std::cout << "add name literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::NameLiteral>();
            literalForm->setName(accumulator);
            add_form_element(literalForm);
            state = ParserState::IN_WHITESPACE;
        }  else if (std::regex_match(character, MATCH_CLOSE_PARENTHESIS)) {
            std::cout << "add name literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::NameLiteral>();
            literalForm->setName(accumulator);
            add_form_element(literalForm);
//            std::cout << "unnest stuff " << top->to_str() << std::endl;
            unnest();
            state = ParserState::IN_WHITESPACE;
        } else if (std::regex_match(character, MATCH_CLOSE_LIST)) {
            std::cout << "add name literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::NameLiteral>();
            literalForm->setName(accumulator);
            add_form_element(literalForm);
//            std::cout << "unnest stuff " << top->to_str() << std::endl;
            unnest();
            state = ParserState::IN_WHITESPACE;
        } else {
            throw std::runtime_error(std::to_string(column) + ":" + std::to_string(line) + " expected name character but got something else");
        }

        return std::shared_ptr<AST::Form>();
    }

    std::shared_ptr<AST::Form> parse_integer(std::string &character)
    {
        std::cout << "IN_INTEGER_LITERAL" << std::endl;

        if (std::regex_match(character, MATCH_INTEGER_LITERAL_CHARACTER)) {
            accumulator += character;
        } else if (std::regex_match(character, MATCH_WHITESPACE)) {
            std::cout << "add integer literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::IntegerLiteral>();
            literalForm->setValue(std::stoi(accumulator));
            add_form_element(literalForm);
            state = ParserState::IN_WHITESPACE;
        }  else if (std::regex_match(character, MATCH_CLOSE_PARENTHESIS)) {
            std::cout << "add integer literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::IntegerLiteral>();
            literalForm->setValue(std::stoi(accumulator));
            add_form_element(literalForm);
//            auto top = formStack.back();
//            formStack.pop_back();
//            std::cout << "unnest stuff " << top->to_str() << std::endl;
            unnest();
            state = ParserState::IN_WHITESPACE;
        } else if (std::regex_match(character, MATCH_CLOSE_LIST)) {
            std::cout << "add integer literal to program: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::IntegerLiteral>();
            literalForm->setValue(std::stoi(accumulator));
            add_form_element(literalForm);
//            auto top = formStack.back();
//            formStack.pop_back();
//            std::cout << "unnest stuff " << top->to_str() << std::endl;
            unnest();
            state = ParserState::IN_WHITESPACE;
        }else {
            throw std::runtime_error(std::to_string(column) + ":" + std::to_string(line) + " expected integer character but got something else");
        }

        return std::shared_ptr<AST::Form>();
    }

    std::shared_ptr<AST::Form> parse_string(std::string &character)
    {
        std::cout << "IN_STRING_LITERAL" << std::endl;

        if (std::regex_match(character, MATCH_STRING_LITERAL_QUOTE)) {
            std::cout << "add string literal to program and exit string: " << accumulator << std::endl;
            auto literalForm = std::make_shared<AST::StringLiteral>();
            literalForm->setText(accumulator);
            add_form_element(literalForm);
            state = ParserState::IN_WHITESPACE;
        } else {
            accumulator += character;
        }

        return std::shared_ptr<AST::Form>();
    }

    std::shared_ptr<AST::List> makeImplicitDo()
    {
        auto implicitToplevelDo = std::make_shared<AST::List>();
        auto implicitToplevelDoName = std::make_shared<AST::NameLiteral>();
        implicitToplevelDoName->setName("do");
        implicitToplevelDo->addElement(implicitToplevelDoName);

        return implicitToplevelDo;
    }

	std::shared_ptr<Program> parse()
	{
		auto newProgram = std::make_shared<Program>();
		state = ParserState::IN_WHITESPACE;
		accumulator = "";
        column = line = 1;
        formStack.clear();

        formStack.push_back(makeImplicitDo());

		while (true) {
			auto optionalCharacter = source->readChar();
			if (optionalCharacter) {
				auto character = optionalCharacter.value();
                std::shared_ptr<AST::Form> form;
                line++;

				switch (state) {
					case ParserState::IN_WHITESPACE:      form = parse_whitespace(character); break;
					case ParserState::IN_NAME_LITERAL:    form = parse_name(character);       break;
					case ParserState::IN_INTEGER_LITERAL: form = parse_integer(character);    break;
					case ParserState::IN_STRING_LITERAL:  form = parse_string(character);     break;
				}

                if (std::regex_match(character, MATCH_LINE_END)) {
                    column++;
                    line = 1;
                }

                if (form) {
                    std::cout << "GOT FORM: " << form->to_str() << std::endl;
                }
			} else {
				break;
			}
		}

        std::cout << "Final program: " << formStack.back()->to_str() << std::endl;

		return newProgram;
	}

	public:
		Parser(std::shared_ptr<Source> source) :
			source(source), program(nullptr)
		{

		}

		std::shared_ptr<Program> getProgram()
		{
			if (!program) {
				program = parse();
			}

			return program;
		}
};