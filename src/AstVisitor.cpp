#include "AstVisitor.h"

#include <iostream>

#include <llvm/IR/Constant.h>

llvm::Value* AstVisitor::visit(const AST::VariableIdentifier& expression) {
    std::cout << "Building variable identifier: " << expression.name << "\n";
    return nullptr;
}

llvm::Value* AstVisitor::visit(const AST::ProgramExpression& expression) {
    std::cout << "Building program expression." << "\n";
    for (auto& ex : expression.expressions) {
        ex->generate(*this);
    }

    return nullptr;
}

llvm::Value* AstVisitor::visit(const AST::VariableDeclarationExpression& expression) {
    std::cout << "Building variable declaration expression: \'" << expression.identifier << "\' of type \'" << expression.variableType << "\'\n";
    return nullptr;
}

llvm::Value* AstVisitor::visit(const AST::AssignExpression& expression) {
    std::cout << "Building assign expression: \'" << expression.identifier->name << "\'\n";
    return nullptr;
}

llvm::Value* AstVisitor::visit(const AST::Int32LiteralExpression& expression) {
    std::cout << "Building int32 literal expression: " << expression.value << "\n";
    return nullptr;
}

