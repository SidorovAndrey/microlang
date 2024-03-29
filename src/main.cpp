#include <iterator>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "common/Result.h"
#include "common/Log.h"
#include "front/Lexer.h"
#include "front/AST.h"
#include "front/AstBuilder.h"
#include "back/AstVisitor.h"

void logLexerToken(const std::vector<Lexer::Token>& tokens) {
    for (const auto& token : tokens) {
        std::string formatted = "id: " + std::to_string(token.id);
        formatted += "\t type: " + Lexer::getTokenTypeName(token.type);
        formatted += "\t at: [" + std::to_string(token.row) + ";" + std::to_string(token.column) + "]";
        formatted += "\t symbol: " + token.symbol;
        Log::write(Log::INFO, formatted);
    }
}

[[nodiscard]] int main(int argc, char** argv) {
    Log::initGlobal(std::make_unique<Log::ConsoleLogger>());

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

    Result<AST::ProgramExpression> treeResult = AstBuilder::buildTree(lexerResult.data);
    if (!treeResult.isSuccess) {
        Log::write(Log::ERROR, treeResult.errorMessage);
        return -1;
    }

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

