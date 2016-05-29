#pragma once

#include <memory>
#include <vector>
#include <set>
#include <string>
#include <regex>

#include "../Sources/Source.h"

namespace Lexer
{
    enum class TokenType
    {
        OpenParen,
        CloseParen,
        Name,
        Number,
        String,
        Comment,
        ReaderDispatch,
        Whitespace,
        Newline,
        Unknown
    };

    struct SourceLocation
    {
        int line;
        int column;
    };

    struct SourceSpan
    {
        SourceLocation start;
        SourceLocation end;
    };

    struct Token
    {
        TokenType type;
        std::string content;
        SourceSpan span;

        std::string toString();
    };

    class Lexer
    {
            std::shared_ptr<std::vector<Token>> tokens = nullptr;
            std::string currentTokenContent = "";
            TokenType currentTokenType = TokenType::Unknown;
            int lastLine = 1;
            int lastColumn = 1;
            int line = 1;
            int column = 1;
            std::shared_ptr<Sources::Source> source;

        public:
            Lexer(std::shared_ptr<Sources::Source> source);
            void addCurrentToken();
            std::shared_ptr<std::vector<Token>> getTokens();
    };
}