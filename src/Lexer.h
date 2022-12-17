#pragma once

#include <string>
#include <vector>

#include "Result.h"

namespace Lexer {
    enum class TokenType {
        IDENTIFIER,
        INT,
        ASSIGN,
        ENDLINE,
        PLUS,
        MINUS,
        MULTIPLY,
        DIVIDE,
        DECLARE_TYPE,
        UNKNOWN
    };

    struct Token {
        uint32_t id;
        TokenType type;
        uint32_t row;
        uint32_t column;
        std::string symbol;
    };

    Result<std::vector<Token>> parse(const std::string& text);
}
