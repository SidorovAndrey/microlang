#pragma once

#include "Lexer.h"

#include <vector>
#include <string>

#include <llvm/IR/Value.h>

#include "common/Result.h"

class AstVisitor;

namespace AST {
    struct Identifier {
        std::string name;

        Identifier(const std::string& t_name) : name(t_name) {};
        virtual ~Identifier() = default;

        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) = 0;
    };

    struct VariableIdentifier : public Identifier {
        VariableIdentifier(const std::string& name) : Identifier(name) {};
        virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };


    struct Expression {
        virtual ~Expression() = default;
        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) = 0;
    };

    struct ProgramExpression {
        std::vector<std::unique_ptr<Expression>> expressions;

        ProgramExpression() {};
        ProgramExpression(ProgramExpression&& value) : expressions(std::move(value.expressions)) {};
    };

    struct VariableDeclarationExpression : public Expression {
        std::string identifier;
        std::string variableType;

        VariableDeclarationExpression(const std::string& t_identifier, const std::string& t_variableType) : identifier(t_identifier), variableType(t_variableType) {};

        VariableDeclarationExpression(const VariableDeclarationExpression& value)
            : identifier(value.identifier), variableType(value.variableType) {};

        VariableDeclarationExpression(VariableDeclarationExpression&& value)
            : identifier(std::move(value.identifier)), variableType(std::move(value.variableType)) {};

        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    struct AssignExpression : public Expression {
        std::unique_ptr<Identifier> identifier;
        std::unique_ptr<Expression> assignExpression;

        AssignExpression(Identifier* id, Expression* expression) : identifier(id), assignExpression(expression) {};

        AssignExpression(AssignExpression&& value) : identifier(std::move(value.identifier)), assignExpression(std::move(value.assignExpression)) {};

        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };

    struct Int32LiteralExpression : public Expression {
        int32_t value;

        Int32LiteralExpression(int32_t t_value) : value(t_value) {};
        Int32LiteralExpression(const Int32LiteralExpression& t_value) : value(t_value.value) {};
        Int32LiteralExpression(Int32LiteralExpression&& t_value) : value(std::move(t_value.value)) {};

        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
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

        BinaryExpression(BinaryExpressionType t_type, Identifier* t_left, Identifier* t_right) : type(t_type), left(t_left), right(t_right) {};
        BinaryExpression(BinaryExpression&& value) : left(std::move(value.left)), right(std::move(value.right)) {};

        [[nodiscard]] virtual Result<llvm::Value*> generate(AstVisitor& visitor) override;
    };
};

