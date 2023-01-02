#pragma once

#include "Lexer.h"

#include <vector>
#include <string>

#include <llvm/IR/Value.h>

#include "Result.h"

class AstVisitor;

namespace AST {
    struct Identifier {
        std::string name;

        Identifier(const std::string& name) : name(name) {};
        virtual ~Identifier() = default;

        virtual Result<llvm::Value*> generate(AstVisitor& visitor) = 0;
    };

    struct VariableIdentifier : public Identifier {
        VariableIdentifier(const std::string& name) : Identifier(name) {};
        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };


    struct Expression {
        virtual ~Expression() = default;
        virtual Result<llvm::Value*> generate(AstVisitor& visitor) = 0;
    };

    struct ProgramExpression {
        std::vector<std::unique_ptr<Expression>> expressions;

        ProgramExpression() {};
        ProgramExpression(ProgramExpression&& value) : expressions(std::move(value.expressions)) {};
    };

    struct VariableDeclarationExpression : public Expression {
        std::string identifier;
        std::string variableType;

        VariableDeclarationExpression(const std::string& identifier, const std::string& variableType) : identifier(identifier), variableType(variableType) {};

        VariableDeclarationExpression(const VariableDeclarationExpression& value)
            : identifier(value.identifier), variableType(value.variableType) {};

        VariableDeclarationExpression(VariableDeclarationExpression&& value)
            : identifier(std::move(value.identifier)), variableType(std::move(value.variableType)) {};

        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    struct AssignExpression : public Expression {
        std::unique_ptr<Identifier> identifier;
        std::unique_ptr<Expression> assignExpression;

        AssignExpression(Identifier* id, Expression* expression) : identifier(id), assignExpression(expression) {};

        AssignExpression(AssignExpression&& value) : identifier(std::move(value.identifier)), assignExpression(std::move(value.assignExpression)) {};

        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    struct Int32LiteralExpression : public Expression {
        int32_t value;

        Int32LiteralExpression(int32_t value) : value(value) {};
        Int32LiteralExpression(const Int32LiteralExpression& val) : value(val.value) {};
        Int32LiteralExpression(Int32LiteralExpression&& val) : value(std::move(val.value)) {};

        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    enum class BinaryExpressionType {
        Add,
        Subtract,
        Multiply,
        Divide,
    };

    struct BinaryExpression : public Expression {
        BinaryExpressionType type;
        std::unique_ptr<Identifier> left;
        std::unique_ptr<Identifier> right;

        BinaryExpression(BinaryExpressionType type, Identifier* left, Identifier* right) : type(type), left(left), right(right) {};
        BinaryExpression(BinaryExpression&& val) : left(std::move(val.left)), right(std::move(val.right)) {};

        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    Result<ProgramExpression> buildTree(const std::vector<Lexer::Token>& tokens);
};

