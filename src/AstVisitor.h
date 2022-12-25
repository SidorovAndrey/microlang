#pragma once

#include <unordered_map>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "AST.h"

class AstVisitor {
    private:
        std::unique_ptr<llvm::LLVMContext> context;
        std::unique_ptr<llvm::IRBuilder<>> builder;
        std::unique_ptr<llvm::Module> module;

        std::unordered_map<std::string, llvm::AllocaInst*> variables;

    public:
        AstVisitor() noexcept;

        void createProgram(const AST::ProgramExpression& expression);
        void configureTarget();
        void dumpCode();

        llvm::Value* visit(const AST::VariableIdentifier& expression);

        llvm::Value* visit(const AST::VariableDeclarationExpression& expression);
        llvm::Value* visit(const AST::AssignExpression& expression);
        llvm::Value* visit(const AST::Int32LiteralExpression& expression);
};

