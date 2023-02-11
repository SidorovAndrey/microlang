#include "AST.h"

#include <string>

#include "AstVisitor.h"
#include "Utils.h"

#include "Log.h"

namespace AST {
    Result<llvm::Value*> VariableIdentifier::generate(AstVisitor& visitor) {
        return visitor.visit(*this);
    }

    Result<llvm::Value*> VariableDeclarationExpression::generate(AstVisitor& visitor) {
        return visitor.visit(*this);
    }

    Result<llvm::Value*> AssignExpression::generate(AstVisitor& visitor) {
        return visitor.visit(*this);
    }

    Result<llvm::Value*> Int32LiteralExpression::generate(AstVisitor& visitor) {
        return visitor.visit(*this);
    }

    Result<llvm::Value*> BinaryExpression::generate(AstVisitor& visitor) {
        return visitor.visit(*this);
    }

    Result<ProgramExpression> buildTree(const std::vector<Lexer::Token> &tokens) {
        Log::write(Log::DEBUG, "Starting building program tree");
        ProgramExpression program;

        int i = 1;
        while (i < tokens.size()) {
            if (tokens[i].type == Lexer::TokenType::DECLARE_TYPE) {
                std::unique_ptr<Expression> expression = std::make_unique<VariableDeclarationExpression>(tokens[i - 1].symbol, tokens[i + 1].symbol);
                program.expressions.push_back(std::move(expression));

                i += 2; // skip type name token, set on \n
                if (tokens[i].type != Lexer::TokenType::ENDLINE) {
                    return Result<ProgramExpression>::Failure("Declaration operation must be ended with new line", std::move(program));
                }
            } else if (tokens[i].type == Lexer::TokenType::ASSIGN) {
                VariableIdentifier* identifier = new VariableIdentifier(tokens[i - 1].symbol);

                ++i;

                if (tokens[i + 1].type == Lexer::TokenType::ENDLINE) {
                    int32_t value = std::stoi(tokens[i].symbol);
                    Int32LiteralExpression* literal = new Int32LiteralExpression(value);

                    std::unique_ptr<Expression> expression = std::make_unique<AssignExpression>(identifier, literal);
                    program.expressions.push_back(std::move(expression));
                } else if (tokens[i].type == Lexer::TokenType::IDENTIFIER) {
                    // TODO: very naive implimentation
                    // TODO: handle assign from another var here
                    ++i;
                    VariableIdentifier* left = new VariableIdentifier(tokens[i - 1].symbol);
                    VariableIdentifier* right = new VariableIdentifier(tokens[i + 1].symbol);

                    Expression* binaryExpression;
                    if (tokens[i].type == Lexer::TokenType::PLUS) {
                        binaryExpression = new BinaryExpression(BinaryExpressionType::Add, left, right);
                    } else if (tokens[i].type == Lexer::TokenType::MINUS) {
                        binaryExpression = new BinaryExpression(BinaryExpressionType::Subtract, left, right);
                    } else if (tokens[i].type == Lexer::TokenType::MULTIPLY) {
                        binaryExpression = new BinaryExpression(BinaryExpressionType::Multiply, left, right);
                    } else if (tokens[i].type == Lexer::TokenType::DIVIDE) {
                        binaryExpression = new BinaryExpression(BinaryExpressionType::Divide, left, right);
                    } else {
                        return Result<ProgramExpression>::Failure("Not defined binary operation \'" + tokens[i].symbol + "\'", std::move(program));
                    }

                    std::unique_ptr<Expression> expression = std::make_unique<AssignExpression>(identifier, binaryExpression);
                    program.expressions.push_back(std::move(expression));
                    ++i;
                }

                ++i;
                if (tokens[i].type != Lexer::TokenType::ENDLINE) {
                    return Result<ProgramExpression>::Failure("Declaration operation must be ended with new line", std::move(program));
                }
            }

            ++i;
        }

        Log::write(Log::DEBUG, "Building program tree succeeded");
        return Result<ProgramExpression>::Ok(std::move(program));
    };
}

