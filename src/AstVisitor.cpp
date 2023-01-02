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

// TODO: do not redeclare printf each call
void addOutputText(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::string text) {

    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(context));
    llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), printf_arg_types, true);
    llvm::Function* printf_func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), module);
    printf_func->setCallingConv(llvm::CallingConv::C);

    llvm::Constant* str = llvm::ConstantDataArray::getString(context, text);
    llvm::GlobalVariable* strVar = new llvm::GlobalVariable(module, llvm::ArrayType::get(llvm::IntegerType::get(context, 8), text.size() + 1 /*+1 is \0 at the end*/), true, llvm::GlobalValue::PrivateLinkage, str, ".str");

    llvm::CallInst* call = builder.CreateCall(printf_func, strVar);
    call->setTailCall(false);
}

void addOutputTextForValue(llvm::LLVMContext& context, llvm::IRBuilder<>& builder, llvm::Module& module, std::string text, llvm::Value* intVal) {

    std::vector<llvm::Type*> printf_arg_types;
    printf_arg_types.push_back(llvm::Type::getInt8PtrTy(context));
    llvm::FunctionType* printf_type = llvm::FunctionType::get(llvm::Type::getInt32Ty(context), printf_arg_types, true);
    llvm::Function* printf_func = llvm::Function::Create(printf_type, llvm::Function::ExternalLinkage, llvm::Twine("printf"), module);
    printf_func->setCallingConv(llvm::CallingConv::C);

    llvm::Constant* str = llvm::ConstantDataArray::getString(context, text);
    llvm::GlobalVariable* strVar = new llvm::GlobalVariable(module, llvm::ArrayType::get(llvm::IntegerType::get(context, 8), text.size() + 1 /*+1 is \0 at the end*/), true, llvm::GlobalValue::PrivateLinkage, str, ".str");

    std::vector<llvm::Value*> params = { strVar, intVal };

    llvm::CallInst* call = builder.CreateCall(printf_func, params);
    call->setTailCall(false);
}

VoidResult AstVisitor::createProgram(const AST::ProgramExpression& expression) {
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
        auto res = ex->generate(*this);
        if (!res.isSuccess) {
            std::cout << "COMPILATION ERROR: " << res.errorMessage << "\n";
            break;
        }
    }

    //addOutputText(*context, *builder, *module, "Hello world!\n");
    //addOutputTextForValue(*context, *builder, *module, "myVar3 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar3"]));
    //addOutputTextForValue(*context, *builder, *module, "myVar4 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar4"]));
    //addOutputTextForValue(*context, *builder, *module, "myVar5 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar5"]));
    addOutputTextForValue(*context, *builder, *module, "myVar6 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar6"]));

    llvm::APInt returnValue(32, (uint32_t)0, true);
    builder->CreateRet(llvm::ConstantInt::get(*context, returnValue));
    llvm::verifyFunction(*main);

    return VoidResult::Ok();
}

void AstVisitor::configureTarget() {
    auto target = llvm::sys::getDefaultTargetTriple();
    module->setTargetTriple(target);
}

std::string AstVisitor::dumpCode() {
    std::string result;
    llvm::raw_string_ostream stream(result);
    module->print(stream, nullptr);
    return result;
}

Result<llvm::Value*> AstVisitor::visit(const AST::VariableIdentifier& expression) {
    std::cout << "Building variable identifier: " << expression.name << "\n";

    llvm::Value* value = variables[expression.name];
    if (!value) {
        return Result<llvm::Value*>::Failure("Cannot find value for \'" + expression.name + '\'', nullptr);
    }

    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::VariableDeclarationExpression& expression) {
    std::cout << "Building variable declaration expression: \'" << expression.identifier << "\' of type \'" << expression.variableType << "\'\n";

    llvm::Value* value = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), 0);

    // TODO: declaration expression must contain declaration of var with concrete type, like Int32VarDeclarationExpression
    // call it like expression.declaration->codeget(*this);

    // figure out how vars should be declared properly 
    llvm::AllocaInst* var = builder->CreateAlloca(
        llvm::Type::getInt32Ty(*context), nullptr, llvm::Twine(expression.identifier));

    variables[expression.identifier] = var;
    builder->CreateStore(value, var);
    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::AssignExpression& expression) {
    std::cout << "Building assign expression: \'" << expression.identifier->name << "\'\n";

    Result<llvm::Value*> value = expression.assignExpression->generate(*this);
    if (!value.isSuccess) {
        return std::move(value);
    }

    Result<llvm::Value*> id = expression.identifier->generate(*this);
    if (!id.isSuccess) {
        return std::move(id);
    }

    builder->CreateStore(value.data, id.data);
    return Result<llvm::Value*>::Ok(value.data);
}

Result<llvm::Value*> AstVisitor::visit(const AST::Int32LiteralExpression& expression) {
    std::cout << "Building int32 literal expression: " << expression.value << "\n";

    llvm::Value* value = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*context), expression.value);
    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::BinaryExpression& expression) {
    std::cout << "Building binary expression: " << expression.left->name << " + " << expression.right->name << "\n";
    Result<llvm::Value*> left = expression.left->generate(*this);
    if (!left.isSuccess) {
        return std::move(left);
    }

    Result<llvm::Value*> right = expression.right->generate(*this);
    if (!right.isSuccess) {
        return std::move(right);
    }

    llvm::Value* result;

    llvm::Value* loadedLeft = builder->CreateLoad(llvm::Type::getInt32Ty(*context), left.data);
    llvm::Value* loadedRight = builder->CreateLoad(llvm::Type::getInt32Ty(*context), right.data);

    switch (expression.type) {
        case AST::BinaryExpressionType::Add:
            result = builder->CreateAdd(loadedLeft, loadedRight, "add");
            break;
        case AST::BinaryExpressionType::Subtract:
            result = builder->CreateSub(loadedLeft, loadedRight, "sub");
            break;
        case AST::BinaryExpressionType::Multiply:
            result = builder->CreateMul(loadedLeft, loadedRight, "mul");
            break;
        case AST::BinaryExpressionType::Divide:
            result = builder->CreateSDiv(loadedLeft, loadedRight, "div");
            break;
    };

    return Result<llvm::Value*>::Ok(result);
}

