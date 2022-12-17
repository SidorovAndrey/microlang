#pragma once

#include "Lexer.h"

#include <vector>
#include <string>

#include "Result.h"

namespace AST {
    enum class NodeType {
        PROGRAM_ROOT,
        INT_LITERAL,
        ASSIGN, // at the right of '=' operator
        FUNCTION_CALL, // '<name>()' call
        VARIABLE_DECLARATION, // by the both side of ':'
        VARIABLE,
        PLUS_OPERATION, // by the both side of '+'
        MINUS_OPERATION, // by the both side of '-'
        MULTIPLY_OPERATION, // by the both side of '*'
        DIVIDE_OPERATION, // by the both side of '*'
        // TODO: how to represent (...) operation (brackets in arithmetic operation)?
    };

    struct Node {
        NodeType type;
        std::string name;
        std::vector<uint8_t> value;
        std::vector<Node> children;
    };

    Result<Node> buildTree(const std::vector<Lexer::Token>& tokens);
}

