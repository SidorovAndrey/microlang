#pragma once

#include <vector>

#include "common/Result.h"
#include "front/AST.h"

namespace AstBuilder {
    [[nodiscard]] Result<AST::ProgramExpression> buildTree(const std::vector<Lexer::Token>& tokens);
}
