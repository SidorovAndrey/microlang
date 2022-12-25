#include "AstVisitor.h"

#include <iostream>

#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Host.h>

AstVisitor::AstVisitor() noexcept {
    context = std::make_unique<llvm::LLVMContext>();
    builder = std::unique_ptr<llvm::IRBuilder<>>(new llvm::IRBuilder<>(*context));
    module = std::make_unique<llvm::Module>("Module", *context);
}

void AstVisitor::createProgram(const AST::ProgramExpression& expression) {
    std::cout << "Building program expression." << "\n";

    llvm::FunctionType* mainType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*context),
        std::vector<llvm::Type*>(), false);

    llvm::Function* main = llvm::Function::Create(
        mainType, llvm::Function::ExternalLinkage, "main", module.get());

    llvm::BasicBlock* mainBasicBlock = llvm::BasicBlock::Create(*context, "entry", main);
    builder->SetInsertPoint(mainBasicBlock);

    variables.clear();

    for (auto& ex : expression.expressions) {
        ex->generate(*this);
    }

    // testing printf lined from c -- it has segfault err
    //llvm::Function* printf = module->getFunction("printf");
    //std::vector<llvm::Value*> args = {
    //    builder->CreateGlobalStringPtr("test hello world", "hwstring")
    //};
    //builder->CreateCall(printf->getFunctionType(), printf, args, "printf");

    llvm::APInt returnValue(32, (uint32_t)0, true);
    builder->CreateRet(llvm::ConstantInt::get(*context, returnValue));
    llvm::verifyFunction(*main);
}

void AstVisitor::configureTarget() {
    auto target = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(target);
}

void AstVisitor::dumpCode() {
    module->print(llvm::outs(), nullptr);
}

llvm::Value* AstVisitor::visit(const AST::VariableIdentifier& expression) {
    std::cout << "Building variable identifier: " << expression.name << "\n";

    llvm::Value* value = variables[expression.name];
    // TODO: handle if value not found; handle all the errors
    return value;
}

llvm::Value* AstVisitor::visit(const AST::VariableDeclarationExpression& expression) {
    std::cout << "Building variable declaration expression: \'" << expression.identifier << "\' of type \'" << expression.variableType << "\'\n";

    llvm::Value* value = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0);

    // TODO: declaration expression must contain declaration of var with concrete type, like Int32VarDeclarationExpression
    // call it like expression.declaration->codeget(*this);

    // figure out how vars should be declared properly 
    llvm::AllocaInst* var = builder->CreateAlloca(
        llvm::Type::getInt32Ty(*context), nullptr, llvm::Twine(expression.identifier));

    variables[expression.identifier] = var;
    builder->CreateStore(value, var);
    return value;
}

llvm::Value* AstVisitor::visit(const AST::AssignExpression& expression) {
    std::cout << "Building assign expression: \'" << expression.identifier->name << "\'\n";

    llvm::Value* val = expression.assignExpression->generate(*this);
    llvm::Value* id = expression.identifier->generate(*this);

    builder->CreateStore(val, id);
    return val;
}

llvm::Value* AstVisitor::visit(const AST::Int32LiteralExpression& expression) {
    std::cout << "Building int32 literal expression: " << expression.value << "\n";

    llvm::Value* val = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), expression.value);
    return val;
}

