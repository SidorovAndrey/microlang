#include <iterator>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Lexer.h"
#include "AST.h"
#include "AstVisitor.h"

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

[[nodiscard]] int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "ERROR: Source file not specified\n");
    }

    std::ifstream file(argv[1]);
    std::string text;
    if (file.is_open()) {
        text = std::string(std::istreambuf_iterator<char>{file}, {});
        file.close();
    }

    Result<std::vector<Lexer::Token>> lexerResult = Lexer::parse(text);
    if (!lexerResult.isSuccess) {
        fprintf(stderr, "%s\n", lexerResult.errorMessage.c_str());
    } else {
        for (int i = 0; i < lexerResult.data.size(); ++i) {
            printf("id: %d\t type: %-15s\t at: [%d;%d]\t symbol: %s\n", lexerResult.data[i].id, getTokenTypeName(lexerResult.data[i].type).c_str(), lexerResult.data[i].row, lexerResult.data[i].column, lexerResult.data[i].symbol.c_str());
        }
    }

    Result<AST::ProgramExpression> treeResult = AST::buildTree(lexerResult.data);

    AstVisitor visitor;
    VoidResult programResult = visitor.createProgram(treeResult.data);
    if (!programResult.isSuccess) {
        fprintf(stderr, "%s\n", programResult.errorMessage.c_str());
        return -1;
    }
    visitor.configureTarget();
    std::string result = visitor.dumpCode();
    printf("%s\n", result.c_str());

    std::string outputFile = argc < 3 ? "./" + std::string(argv[1]) + ".ll" : std::string(argv[2]);
    std::ofstream writeFile(outputFile);
    writeFile << result;
}

