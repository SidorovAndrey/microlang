#pragma once

#include <unordered_map>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "AST.h"
#include "Result.h"

class AstVisitor {
private:
    std::unique_ptr<llvm::LLVMContext> m_context;
    std::unique_ptr<llvm::IRBuilder<>> m_builder;
    std::unique_ptr<llvm::Module> m_module;

    std::unordered_map<std::string, llvm::AllocaInst*> m_variables;

public:
    AstVisitor() noexcept;

    [[nodiscard]] VoidResult createProgram(const AST::ProgramExpression& expression);
    void configureTarget();
    [[nodiscard]] std::string dumpCode();

    [[nodiscard]] Result<llvm::Value*> visit(const AST::VariableIdentifier& expression);

    [[nodiscard]] Result<llvm::Value*> visit(const AST::VariableDeclarationExpression& expression);
    [[nodiscard]] Result<llvm::Value*> visit(const AST::AssignExpression& expression);
    [[nodiscard]] Result<llvm::Value*> visit(const AST::Int32LiteralExpression& expression);
    [[nodiscard]] Result<llvm::Value*> visit(const AST::BinaryExpression& expression);
};

