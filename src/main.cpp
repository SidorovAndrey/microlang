#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Lexer.h"
#include "AST.h"
#include "AstVisitor.h"
#include "Log.h"

[[nodiscard]] std::string getTokenTypeName(const Lexer::TokenType type) {
    switch(type) {
        case Lexer::TokenType::IDENTIFIER:
            return "IDENTIFIER";
        case Lexer::TokenType::INT:
            return "INT";
        case Lexer::TokenType::ASSIGN:
            return "ASSIGN";
        case Lexer::TokenType::ENDLINE:
            return "ENDLINE";
        case Lexer::TokenType::PLUS:
            return "PLUS";
        case Lexer::TokenType::MINUS:
            return "MINUS";
        case Lexer::TokenType::MULTIPLY:
            return "MULTIPLY";
        case Lexer::TokenType::DIVIDE:
            return "DIVIDE";
        case Lexer::TokenType::DECLARE_TYPE:
            return "DECLARE_TYPE";
        default:
            return "UNKNOWN";
    }
}

void logLexerToken(const std::vector<Lexer::Token>& tokens) {
    for (const auto& token : tokens) {
        // TODO: find better way for such formatting
        std::string formatted = "id: " + std::to_string(token.id);
        formatted += "\t type: " + getTokenTypeName(token.type);
        formatted += "\t at: [" + std::to_string(token.row) + ";" + std::to_string(token.column) + "]";
        formatted += "\t symbol: " + token.symbol;
        Log::write(Log::INFO, formatted);
    }
}

[[nodiscard]] int main(int argc, char** argv) {
    Log::initGlobal(&std::cout); // TODO: not sure about how to pass std::cout here

    if (argc < 2) {
        Log::write(Log::ERROR, "Source file not specified");
        return -1;
    }

    std::string fileName = argv[1];
    std::ifstream file(fileName);
    std::string text;
    if (file.is_open()) {
        Log::write(Log::INFO, "Reading source file \'" + fileName + "\'");
        text = std::string(std::istreambuf_iterator<char>{file}, {});
        file.close();
        Log::write(Log::INFO, "Reading source file \'" + fileName + "\' succeeded");
    }

    Result<std::vector<Lexer::Token>> lexerResult = Lexer::parse(text);
    if (!lexerResult.isSuccess) {
        Log::write(Log::ERROR, lexerResult.errorMessage);
        return -1;
    } else {
        logLexerToken(lexerResult.data);
        //for (int i = 0; i < lexerResult.data.size(); ++i) {
        //    printf("id: %d\t type: %-15s\t at: [%d;%d]\t symbol: %s\n", lexerResult.data[i].id, getTokenTypeName(lexerResult.data[i].type).c_str(), lexerResult.data[i].row, lexerResult.data[i].column, lexerResult.data[i].symbol.c_str());
        //}
    }

    Result<AST::ProgramExpression> treeResult = AST::buildTree(lexerResult.data);

    AstVisitor visitor;
    VoidResult programResult = visitor.createProgram(treeResult.data);
    if (!programResult.isSuccess) {
        Log::write(Log::ERROR, programResult.errorMessage);
        return -1;
    }
    visitor.configureTarget();
    std::string result = visitor.dumpCode();
    Log::write(Log::DEBUG, "LLVM IR Code dump:\n" + result + "\n");

    std::string outputFile = argc < 3 ? "./" + std::string(argv[1]) + ".ll" : std::string(argv[2]);
    Log::write(Log::INFO, "Writing LLVM IR Code into file \'" + outputFile + "\'");
    std::ofstream writeFile(outputFile);
    writeFile << result;
    Log::write(Log::INFO, "Writing LLVM IR Code into file \'" + outputFile + "\' succeeded");
}

