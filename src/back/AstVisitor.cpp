#include "AstVisitor.h"

#include <llvm/IR/Constant.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <llvm/Support/Host.h>

#include "Log.h"

AstVisitor::AstVisitor() noexcept {
    m_context = std::make_unique<llvm::LLVMContext>();
    m_builder = std::unique_ptr<llvm::IRBuilder<>>(new llvm::IRBuilder<>(*m_context));
    m_module = std::make_unique<llvm::Module>("Module", *m_context);
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
    Log::write(Log::DEBUG, "Building program expression");

    llvm::FunctionType* mainType = llvm::FunctionType::get(
        llvm::IntegerType::getInt32Ty(*m_context),
        std::vector<llvm::Type*>(), false);

    llvm::Function* main = llvm::Function::Create(
        mainType, llvm::Function::ExternalLinkage, "main", m_module.get());

    llvm::BasicBlock* mainBasicBlock = llvm::BasicBlock::Create(*m_context, "entry", main);
    m_builder->SetInsertPoint(mainBasicBlock);

    m_variables.clear();

    for (auto& ex : expression.expressions) {
        auto res = ex->generate(*this);
        if (!res.isSuccess) {
            return VoidResult::Failure("COMPILATION ERROR: " + res.errorMessage);
        }
    }

    //addOutputText(*context, *builder, *module, "Hello world!\n");
    //addOutputTextForValue(*context, *builder, *module, "myVar3 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar3"]));
    //addOutputTextForValue(*context, *builder, *module, "myVar4 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar4"]));
    //addOutputTextForValue(*context, *builder, *module, "myVar5 = %d\n", builder->CreateLoad(llvm::Type::getInt32Ty(*context), variables["myVar5"]));
    addOutputTextForValue(*m_context, *m_builder, *m_module, "myVar6 = %d\n", m_builder->CreateLoad(llvm::Type::getInt32Ty(*m_context), m_variables["myVar6"]));

    llvm::APInt returnValue(32, (uint32_t)0, true);
    m_builder->CreateRet(llvm::ConstantInt::get(*m_context, returnValue));
    llvm::verifyFunction(*main);

    return VoidResult::Ok();
}

void AstVisitor::configureTarget() {
    auto target = llvm::sys::getDefaultTargetTriple();
    m_module->setTargetTriple(target);
}

std::string AstVisitor::dumpCode() {
    std::string result;
    llvm::raw_string_ostream stream(result);
    m_module->print(stream, nullptr);
    return result;
}

Result<llvm::Value*> AstVisitor::visit(const AST::VariableIdentifier& expression) {
    Log::write(Log::DEBUG, "Building variable identifier: " + expression.name);

    llvm::Value* value = m_variables[expression.name];
    if (!value) {
        return Result<llvm::Value*>::Failure("Cannot find value for \'" + expression.name + '\'', nullptr);
    }

    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::VariableDeclarationExpression& expression) {
    Log::write(Log::DEBUG, "Building variable declaration expression: \'" + expression.identifier + "\' of type \'" + expression.variableType);

    llvm::Value* value = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*m_context), 0);

    // TODO: declaration expression must contain declaration of var with concrete type, like Int32VarDeclarationExpression
    // call it like expression.declaration->codeget(*this);

    // figure out how vars should be declared properly 
    llvm::AllocaInst* var = m_builder->CreateAlloca(
        llvm::Type::getInt32Ty(*m_context), nullptr, llvm::Twine(expression.identifier));

    m_variables[expression.identifier] = var;
    m_builder->CreateStore(value, var);
    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::AssignExpression& expression) {
    Log::write(Log::DEBUG, "Building assign expression: \'" + expression.identifier->name + "\'");

    Result<llvm::Value*> value = expression.assignExpression->generate(*this);
    if (!value.isSuccess) {
        return value;
    }

    Result<llvm::Value*> id = expression.identifier->generate(*this);
    if (!id.isSuccess) {
        return id;
    }

    m_builder->CreateStore(value.data, id.data);
    return Result<llvm::Value*>::Ok(value.data);
}

Result<llvm::Value*> AstVisitor::visit(const AST::Int32LiteralExpression& expression) {
    Log::write(Log::DEBUG, "Building int32 literal expression: " + std::to_string(expression.value));

    llvm::Value* value = llvm::ConstantInt::getSigned(llvm::Type::getInt32Ty(*m_context), expression.value);
    return Result<llvm::Value*>::Ok(value);
}

Result<llvm::Value*> AstVisitor::visit(const AST::BinaryExpression& expression) {
    Log::write(Log::DEBUG, "Building binary expression: " + expression.left->name + " + " + expression.right->name);
    Result<llvm::Value*> left = expression.left->generate(*this);
    if (!left.isSuccess) {
        return left;
    }

    Result<llvm::Value*> right = expression.right->generate(*this);
    if (!right.isSuccess) {
        return right;
    }

    llvm::Value* result;

    llvm::Value* loadedLeft = m_builder->CreateLoad(llvm::Type::getInt32Ty(*m_context), left.data);
    llvm::Value* loadedRight = m_builder->CreateLoad(llvm::Type::getInt32Ty(*m_context), right.data);

    switch (expression.type) {
        case AST::BinaryExpressionType::Add:
            result = m_builder->CreateAdd(loadedLeft, loadedRight, "add");
            break;
        case AST::BinaryExpressionType::Subtract:
            result = m_builder->CreateSub(loadedLeft, loadedRight, "sub");
            break;
        case AST::BinaryExpressionType::Multiply:
            result = m_builder->CreateMul(loadedLeft, loadedRight, "mul");
            break;
        case AST::BinaryExpressionType::Divide:
            result = m_builder->CreateSDiv(loadedLeft, loadedRight, "div");
            break;
    };

    return Result<llvm::Value*>::Ok(result);
}

