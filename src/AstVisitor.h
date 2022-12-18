#pragma once

#include <llvm/IR/Value.h>

#include "AST.h"

class AstVisitor {
    public:
        llvm::Value* visit(const AST::VariableIdentifier& expression);

        llvm::Value* visit(const AST::ProgramExpression& expression);
        llvm::Value* visit(const AST::VariableDeclarationExpression& expression);
        llvm::Value* visit(const AST::AssignExpression& expression);
        llvm::Value* visit(const AST::Int32LiteralExpression& expression);
};

