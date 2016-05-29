#include "Lexer.h"

namespace Lexer
{
    std::set<std::string> OPEN_PARENS{ "(", "[" },
        CLOSE_PARENS{ ")", "]" };
    std::regex NAME_FIRST_CHARACTER{ R"([^0-9.()\[\]#"\s])" },
        NAME_NEXT_CHARACTER{R"([^()\[\]\s#".])"},
        NUMBER_FIRST_CHARACTER{ R"([0-9-])" },
        NUMBER_NEXT_CHARACTER{ R"([0-9.])" },
        WHITESPACE_CHARACTER{ R"(\s+)" },
        NEWLINE_CHARACTER{ R"(\n)" };
    std::string COMMENT_CHARACTER = ";",
        STRING_CHARACTER = "\"",
        READER_DISPATCH = "#";

    bool isSeparator(TokenType tokenType)
    {
        // if (OPEN_PARENS.find(character) != OPEN_PARENS.end()) return true;
        // if (CLOSE_PARENS.find(character) != CLOSE_PARENS.end()) return true;
        // if (std::regex_match(character, NEWLINE_CHARACTER)) return true;
        // if (std::regex_match(character, WHITESPACE_CHARACTER)) return true;
        // if (character == COMMENT_CHARACTER) return true;
        // if (character == STRING_CHARACTER) return true;
        // if (character == READER_DISPATCH) return true;

        // return false;

        if (tokenType == TokenType::OpenParen)      return true;
        if (tokenType == TokenType::CloseParen)     return true;
        // if (tokenType == TokenType::Name)           return true;
        if (tokenType == TokenType::Number)         return true;
        if (tokenType == TokenType::Newline)        return true;
        if (tokenType == TokenType::Whitespace)     return true;
        if (tokenType == TokenType::Comment)        return true;
        if (tokenType == TokenType::String)         return true;
        if (tokenType == TokenType::ReaderDispatch) return true;

        return false;
    }

    TokenType appraiseTokenType(std::string character)
    {
        if (OPEN_PARENS.find(character) != OPEN_PARENS.end()) return TokenType::OpenParen;
        if (CLOSE_PARENS.find(character) != CLOSE_PARENS.end()) return TokenType::CloseParen;
        if (character == COMMENT_CHARACTER) return TokenType::Comment;
        if (character == STRING_CHARACTER) return TokenType::String;
        if (character == READER_DISPATCH) return TokenType::ReaderDispatch;
        if (std::regex_match(character, NAME_FIRST_CHARACTER)) return TokenType::Name;
        if (std::regex_match(character, NAME_NEXT_CHARACTER)) return TokenType::Name;
        if (std::regex_match(character, NUMBER_FIRST_CHARACTER)) return TokenType::Number;
        if (std::regex_match(character, NEWLINE_CHARACTER)) return TokenType::Newline;
        if (std::regex_match(character, WHITESPACE_CHARACTER)) return TokenType::Whitespace;

        return TokenType::Unknown;
    }

    std::string tokenTypeToString(TokenType value)
    {
        if (value == TokenType::OpenParen)      return "OpenParen";
        if (value == TokenType::CloseParen)     return "CloseParen";
        if (value == TokenType::Name)           return "Name";
        if (value == TokenType::Number)         return "Number";
        if (value == TokenType::String)         return "String";
        if (value == TokenType::Comment)        return "Comment";
        if (value == TokenType::ReaderDispatch) return "ReaderDispatch";
        if (value == TokenType::Whitespace)     return "Whitespace";
        if (value == TokenType::Newline)        return "Newline";

        return "Unknown";
    }

    std::string Token::toString()
    {
        return tokenTypeToString(type) +
            "(" + content + ") @ " +
            std::to_string(span.start.line) + ":" + std::to_string(span.start.column);
    }

    Lexer::Lexer(std::shared_ptr<Sources::Source> source) :
        source(source)
    {

    }

    void Lexer::addCurrentToken()
    {
        if (!currentTokenContent.empty())
        {
            if (currentTokenType != TokenType::Whitespace && currentTokenType != TokenType::Newline) {
                tokens->push_back({ currentTokenType, currentTokenContent,{ { lastLine, lastColumn },{ line, column } } });
            }
            lastLine = line;
            lastColumn = column;
        }
    }

    std::shared_ptr<std::vector<Token>> Lexer::getTokens()
    {
        tokens = std::make_shared<std::vector<Token>>();
        currentTokenContent = "";
        currentTokenType = TokenType::Unknown;
        bool stop = false;
        line = 1;
        column = 1;
        bool inComment = false;
        bool inString = false;

        while (true) {
            auto optionalCharacter = source->readChar();

            if (optionalCharacter) {
                auto character = optionalCharacter.value();
                TokenType newTokenType = appraiseTokenType(character);

                if (inComment) {
                    if (newTokenType == TokenType::Newline) {
                        inComment = false;
                    } else {
                        currentTokenContent += character;
                    }
                    continue;
                }

                if (inString) {
                    if (newTokenType == TokenType::String) {
                        inString = false;
                    }
                    else {
                        currentTokenContent += character;
                    }
   
                 continue                        ;
                }

                if (newTokenType != currentTokenType || isSeparator(newTokenType)) {
                    addCurrentToken();

                    if (newTokenType == TokenType::String) {
                        currentTokenContent = "";
                        inString = true;
                    }
                    else if (newTokenType == TokenType::Comment) {
                        currentTokenContent = "";
                        inComment = true;
                    }
                    else {
                        currentTokenContent = character;
                    }
                }
                else {
                    currentTokenContent += character;
                }

                if (newTokenType == TokenType::Newline) {
                    line++;
                    column = 1;
                }
                else {
                    column++;
                }

                currentTokenType = newTokenType;
            }
            else {
                addCurrentToken();
                break;
            }
        }

        return tokens;
    }
}