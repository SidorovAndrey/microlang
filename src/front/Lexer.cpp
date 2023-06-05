#include "front/Lexer.h"

#include <cctype>
#include <string>

#include "common/Log.h"

namespace Lexer {
    void skipIgnoredSymbols(const std::string& text, uint32_t& i) {
        while (i < text.size() && (text[i] == ' ' || text[i] == '\t')) {
            ++i;
        }
    }

    TokenType getOperatorTokenType(const char symbol, std::string& errorMessage, uint32_t row, uint32_t col) {
        errorMessage = "";

        if (symbol == '=') {
            return TokenType::ASSIGN;
        } else if (symbol == '+') {
            return TokenType::PLUS;
        } else if (symbol == '-') {
            return TokenType::MINUS;
        } else if (symbol == '*') {
            return TokenType::MULTIPLY;
        } else if (symbol == '/') {
            return TokenType::DIVIDE;
        } else if (symbol == ':') {
            return TokenType::COLON;
        }

        errorMessage = "unkown token at [" + std::to_string(row) + ":" + std::to_string(col) + "]";
        return TokenType::UNKNOWN;
    }

    std::string parseLine(const std::string& line, std::vector<Token>& tokens, uint32_t row, uint32_t& id) {
        uint32_t col = 0;
        while (col < line.size()) {
            skipIgnoredSymbols(line, col);
            if (col == line.size()) {
                return "";
            }
            if (line[col] == '\n') {
                col++;
                Token token {
                    .id = id++,
                    .type = TokenType::ENDLINE,
                    .row = row,
                    .column = col,
                    .symbol = ""
                };

                tokens.push_back(token);
            } else if (std::isalpha(line[col])) {
                uint32_t colStart = col;
                std::string name = "";
                while (col < line.size() && (std::isalnum(line[col]) || line[col] == '_')) {
                    name += line[col];
                    ++col;
                }

                TokenType tokenType = name == "let" 
                    ? TokenType::DECLARE_VAR 
                    : TokenType::IDENTIFIER;

                Token token {
                    .id = id++,
                    .type = tokenType,
                    .row = row,
                    .column = colStart + 1,
                    .symbol = name
                };
                
                tokens.push_back(token);
            } else if (std::isdigit(line[col])) {
                uint32_t colStart = col;
                std::string value = "";
                while (col < line.size() && (std::isalnum(line[col]) || line[col] == '_')) {
                    value += line[col];
                    ++col;
                }

                Token token {
                    .id = id++,
                    .type = TokenType::INT,
                    .row = row,
                    .column = colStart + 1,
                    .symbol = value
                };
                
                tokens.push_back(token);
            } else if (line[col] == '=' || line[col] == '+' || line[col] == '-' || line[col] == '*' || line[col] == '/' || line[col] == ':') {
                uint32_t colStart = col;
                ++col;

                std::string error;
                TokenType operation = getOperatorTokenType(line[colStart], error, row, colStart);
                if (error != "") {
                    return error;
                }

                Token token {
                    .id = id++,
                    .type = operation,
                    .row = row,
                    .column = colStart + 1,
                    .symbol = std::string(1, line[colStart])
                };
                
                tokens.push_back(token);
            } else {
                return "unkown token at [" + std::to_string(row) + ":" + std::to_string(col) + "]";
            }
        }

        return "";
    }

    std::string getTokenTypeName(const Lexer::TokenType type) {
        switch(type) {
            case Lexer::TokenType::IDENTIFIER:
                return "IDENTIFIER";
            case Lexer::TokenType::INT:
                return "INT";
            case Lexer::TokenType::ASSIGN:
                return "ASSIGN";
            case Lexer::TokenType::ENDLINE:
                return "ENDLINE";
            case Lexer::TokenType::PLUS:
                return "PLUS";
            case Lexer::TokenType::MINUS:
                return "MINUS";
            case Lexer::TokenType::MULTIPLY:
                return "MULTIPLY";
            case Lexer::TokenType::DIVIDE:
                return "DIVIDE";
            case Lexer::TokenType::COLON:
                return "TYPE";
            case Lexer::TokenType::DECLARE_VAR:
                return "DECLARE_VAR";
            default:
                return "UNKNOWN";
        }
    }

    Result<std::vector<Token>> parse(const std::string& text) {
        Log::write(Log::DEBUG, "Starting source code parse");
        std::vector<Token> tokens;

        uint32_t i = 0;
        uint32_t id = 1;
        uint32_t row = 1;
        skipIgnoredSymbols(text, i);
        while (i < text.size()) {
            int lineEnd = text.find('\n', i);
            if (lineEnd == std::string::npos) {
                lineEnd = text.size() - 1;
            }

            std::string line = text.substr(i, lineEnd - i + 1);
            std::string error = parseLine(line, tokens, row, id);
            if (error != "") {
                return Result<std::vector<Token>>::Failure(error, tokens);
            }

            i = lineEnd + 1;
            ++row;
        }

        Log::write(Log::DEBUG, "Source code parse succeeded");
        return Result<std::vector<Token>>::Ok(tokens);
    }
}

