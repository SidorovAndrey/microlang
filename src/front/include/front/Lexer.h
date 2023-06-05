#pragma once

#include <string>
#include <vector>

#include "common/Result.h"

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
        DECLARE_VAR,
        COLON,
        UNKNOWN
    };

    
    [[nodiscard]] std::string getTokenTypeName(const Lexer::TokenType type);
    
    struct Token {
        uint32_t id;
        TokenType type;
        uint32_t row;
        uint32_t column;
        std::string symbol;
    };

    [[nodiscard]] Result<std::vector<Token>> parse(const std::string& text);
}
