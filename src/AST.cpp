#include "AST.h"

#include <string>
#include "Utils.h"

namespace AST {
    Result<Node> buildTree(const std::vector<Lexer::Token>& tokens) {
        Node programRoot {
            .type = NodeType::PROGRAM_ROOT,
            .name = "",
        };

        int i = 1;
        while (i < tokens.size()) {
            if (tokens[i].type == Lexer::TokenType::DECLARE_TYPE) {
                Node varDeclaration {
                    .type = NodeType::VARIABLE_DECLARATION,
                    .name = tokens[i - 1].symbol,
                    .value = std::vector<uint8_t>(tokens[i + 1].symbol.begin(), tokens[i + 1].symbol.end())
                };

                programRoot.children.push_back(varDeclaration);
                
                i += 2; // skip type name token, set on \n
                if (tokens[i].type != Lexer::TokenType::ENDLINE) {
                    return (Result<Node>) {
                        .isSuccess = false,
                        .errorMessage = "Declaration operation must be ended with new line",
                        .data = programRoot
                    };
                }
            } else if (tokens[i].type == Lexer::TokenType::ASSIGN) {
                Node assign {
                    .type = NodeType::ASSIGN,
                    .name = ""
                };

                Node variable {
                    .type = NodeType::VARIABLE,
                    .name = tokens[i - 1].symbol,
                    .value = std::vector<uint8_t>(tokens[i - 1].symbol.begin(), tokens[i - 1].symbol.end())
                };

                ++i;
                assign.children.push_back(variable);

                Node intLiteral {
                    .type = NodeType::INT_LITERAL,
                    .name = tokens[i].symbol,
                    // it should store int value
                    .value = Utils::intToByteArray(std::stoi(tokens[i].symbol))
                };

                assign.children.push_back(intLiteral);
                programRoot.children.push_back(assign);
                
                ++i;
                if (tokens[i].type != Lexer::TokenType::ENDLINE) {
                    return (Result<Node>) {
                        .isSuccess = false,
                        .errorMessage = "Assign operation must be ended with new line",
                        .data = programRoot
                    };
                }
            }

            ++i;
        }

        return (Result<Node>) {
            .isSuccess = true,
            .errorMessage = "",
            .data = programRoot
        };
    }
}

