#include <iterator>
#include <cstdio>
#include <fstream>
#include <string>
#include <vector>

#include "Result.h"
#include "Lexer.h"
#include "AST.h"
#include "AstVisitor.h"

std::string getTokenTypeName(const Lexer::TokenType type) {
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

/*std::string getNodeTypeName(const AST::NodeType type) {
    switch(type) {
        case AST::NodeType::PROGRAM_ROOT:
            return "PROGRAM_ROOT";
        case AST::NodeType::VARIABLE_DECLARATION:
            return "VARIABLE_DECLARATION";
        case AST::NodeType::ASSIGN:
            return "ASSIGN";
        case AST::NodeType::VARIABLE:
            return "VARIABLE";
        case AST::NodeType::INT_LITERAL:
            return "INT_LITERAL";
        default:
            return "UNKNOWN";
    }
}*/

/*void printNode(const AST::Node& node, int nest) {
    printf("%*s type: %s\t name: %s\n", nest, "", getNodeTypeName(node.type).c_str(), node.name.c_str());
    for (AST::Node n : node.children) {
        printNode(n, nest + 1);
    }
}*/

int main(int argc, char** argv) {
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
    visitor.createProgram(treeResult.data);
    visitor.configureTarget();
    visitor.dumpCode();
    // TODO: test treeResult error cases
    //printNode(treeResult.data, 0);


}

