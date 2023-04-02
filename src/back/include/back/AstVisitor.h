#pragma once

#include <unordered_map>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "front/AST.h"
#include "common/Result.h"

class AstVisitor {
private:
    std::unique_ptr<llvm::LLVMContext> m_context;
    std::unique_ptr<llvm::IRBuilder<>> m_builder;
    std::unique_ptr<llvm::Module> m_module;

    std::unordered_map<std::string, llvm::AllocaInst*> m_variables;

public:
    AstVisitor() noexcept;
    virtual ~AstVisitor() = default;

    [[nodiscard]] virtual VoidResult createProgram(const AST::ProgramExpression& expression);
    virtual void configureTarget();
    [[nodiscard]] virtual std::string dumpCode();

    [[nodiscard]] virtual Result<llvm::Value*> visit(const AST::VariableIdentifier& expression);

    [[nodiscard]] virtual Result<llvm::Value*> visit(const AST::VariableDeclarationExpression& expression);
    [[nodiscard]] virtual Result<llvm::Value*> visit(const AST::AssignExpression& expression);
    [[nodiscard]] virtual Result<llvm::Value*> visit(const AST::Int32LiteralExpression& expression);
    [[nodiscard]] virtual Result<llvm::Value*> visit(const AST::BinaryExpression& expression);
};

