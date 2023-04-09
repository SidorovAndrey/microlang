#include "front/AstBuilder.h"

#include "front/Lexer.h"
#include "common/Log.h"

Result<std::unique_ptr<AST::Expression>> processDeclaration(int& idx, const std::vector<Lexer::Token>& tokens) {
    std::unique_ptr<AST::Expression> expression = std::make_unique<AST::VariableDeclarationExpression>(tokens[idx - 1].symbol, tokens[idx + 1].symbol);

    idx += 2; // skip type name token, set on \n
    if (tokens[idx].type != Lexer::TokenType::ENDLINE) {
        return Result<std::unique_ptr<AST::Expression>>::Failure("Declaration operation must be ended with new line", std::move(expression));
    }

    return Result<std::unique_ptr<AST::Expression>>::Ok(std::move(expression));
}

Result<std::unique_ptr<AST::Expression>> processAssign(int& idx, const std::vector<Lexer::Token>& tokens) {
    std::unique_ptr<AST::Expression> result;

    AST::VariableIdentifier* identifier = new AST::VariableIdentifier(tokens[idx - 1].symbol);

    ++idx;

    if (tokens[idx + 1].type == Lexer::TokenType::ENDLINE) {
        int32_t value = std::stoi(tokens[idx].symbol);
        AST::Int32LiteralExpression* literal = new AST::Int32LiteralExpression(value);

        result = std::make_unique<AST::AssignExpression>(identifier, literal);
    } else if (tokens[idx].type == Lexer::TokenType::IDENTIFIER) {
        // TODO: very naive implimentation
        // TODO: handle assign from another var here
        ++idx;
        AST::VariableIdentifier* left = new AST::VariableIdentifier(tokens[idx - 1].symbol);
        AST::VariableIdentifier* right = new AST::VariableIdentifier(tokens[idx + 1].symbol);

        AST::Expression* binaryExpression;
        if (tokens[idx].type == Lexer::TokenType::PLUS) {
            binaryExpression = new AST::BinaryExpression(AST::BinaryExpressionType::Add, left, right);
        } else if (tokens[idx].type == Lexer::TokenType::MINUS) {
            binaryExpression = new AST::BinaryExpression(AST::BinaryExpressionType::Subtract, left, right);
        } else if (tokens[idx].type == Lexer::TokenType::MULTIPLY) {
            binaryExpression = new AST::BinaryExpression(AST::BinaryExpressionType::Multiply, left, right);
        } else if (tokens[idx].type == Lexer::TokenType::DIVIDE) {
            binaryExpression = new AST::BinaryExpression(AST::BinaryExpressionType::Divide, left, right);
        } else {
            return Result<std::unique_ptr<AST::Expression>>::Failure("Not defined binary operation \'" + tokens[idx].symbol + "\'", nullptr);
        }

        ++idx;
        result = std::make_unique<AST::AssignExpression>(identifier, binaryExpression);
    }

    ++idx;
    if (tokens[idx].type != Lexer::TokenType::ENDLINE) {
        return Result<std::unique_ptr<AST::Expression>>::Failure("Declaration operation must be ended with new line", std::move(result));
    }

    return Result<std::unique_ptr<AST::Expression>>::Ok(std::move(result));
}

Result<std::unique_ptr<AST::Expression>> processNext(int& idx, const std::vector<Lexer::Token>& tokens) {
    while (tokens[idx].type != Lexer::TokenType::DECLARE_TYPE && tokens[idx].type != Lexer::TokenType::ASSIGN) {
        ++idx;
        if (idx >= tokens.size()) {
            return Result<std::unique_ptr<AST::Expression>>::Ok(nullptr);
        }
    }
    Log::write(Log::DEBUG, "Processing token with id = " + std::to_string(tokens[idx].id));
    if (tokens[idx].type == Lexer::TokenType::DECLARE_TYPE) {
        return processDeclaration(idx, tokens);
    } else if (tokens[idx].type == Lexer::TokenType::ASSIGN) {
        return processAssign(idx, tokens);
    }

    return Result<std::unique_ptr<AST::Expression>>::Failure("Unknown expression, token id = " + std::to_string(tokens[idx].id), nullptr);
}

Result<AST::ProgramExpression> AstBuilder::buildTree(const std::vector<Lexer::Token>& tokens) {
    Log::write(Log::DEBUG, "Starting building program tree");
    AST::ProgramExpression program;

    int i = 1;
    while (i < tokens.size()) {
        auto result = processNext(i, tokens);
        if (!result.isSuccess) {
            return Result<AST::ProgramExpression>::Failure(result.errorMessage, std::move(program));
        }

        if (result.data != nullptr) {
            program.expressions.push_back(std::move(result.data));
        }
    }

    Log::write(Log::DEBUG, "Building program tree succeeded");
    return Result<AST::ProgramExpression>::Ok(std::move(program));
};

