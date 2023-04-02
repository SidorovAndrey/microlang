#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "front/AST.h"
#include "back/AstVisitor.h"

class AstVisitorMock : public AstVisitor::AstVisitor {
public:
    MOCK_METHOD(VoidResult, createProgram, (const AST::ProgramExpression& expression), (override));
    MOCK_METHOD(void, configureTarget, (), (override));
    MOCK_METHOD(std::string, dumpCode, (), (override));

    MOCK_METHOD(Result<llvm::Value*>, visit, (const AST::VariableIdentifier& expression), (override));
    MOCK_METHOD(Result<llvm::Value*>, visit, (const AST::VariableDeclarationExpression& expression), (override));
    MOCK_METHOD(Result<llvm::Value*>, visit, (const AST::AssignExpression& expression), (override));
    MOCK_METHOD(Result<llvm::Value*>, visit, (const AST::Int32LiteralExpression& expression), (override));
    MOCK_METHOD(Result<llvm::Value*>, visit, (const AST::BinaryExpression& expression), (override));
};

TEST(GenerateLlvmValue, ReturnsResultFromVisitorForVariableIdentifier) {
    AstVisitorMock visitorMock;
    AST::VariableIdentifier identifier("test name");

    ON_CALL(visitorMock, visit(testing::A<const AST::VariableIdentifier&>()))
        .WillByDefault(testing::Return(testing::ByMove(Result<llvm::Value*>::Ok(nullptr))));

    Result<llvm::Value*> result = identifier.generate(visitorMock);

    EXPECT_TRUE(result.isSuccess);
    EXPECT_THAT(result.data, testing::IsNull());
}

TEST(GenerateLlvmValue, ReturnsResultFromVisitorForVariableDeclarationExpression) {
    AstVisitorMock visitorMock;
    AST::VariableDeclarationExpression declaration("identifier", "variable type");

    ON_CALL(visitorMock, visit(testing::A<const AST::VariableDeclarationExpression&>()))
        .WillByDefault(testing::Return(testing::ByMove(Result<llvm::Value*>::Ok(nullptr))));

    Result<llvm::Value*> result = declaration.generate(visitorMock);

    EXPECT_TRUE(result.isSuccess);
    EXPECT_THAT(result.data, testing::IsNull());
}

TEST(GenerateLlvmValue, ReturnsResultFromVisitorForAssignExpression) {
    AstVisitorMock visitorMock;

    auto identifier = new AST::VariableIdentifier("name");
    auto expression = new AST::Int32LiteralExpression(1);
    AST::AssignExpression assign(identifier, expression);

    ON_CALL(visitorMock, visit(testing::A<const AST::AssignExpression&>()))
        .WillByDefault(testing::Return(testing::ByMove(Result<llvm::Value*>::Ok(nullptr))));

    Result<llvm::Value*> result = assign.generate(visitorMock);

    EXPECT_TRUE(result.isSuccess);
    EXPECT_THAT(result.data, testing::IsNull());
}

TEST(GenerateLlvmValue, ReturnsResultFromVisitorForInt32LiteralExpression) {
    AstVisitorMock visitorMock;

    AST::Int32LiteralExpression literal(1);

    ON_CALL(visitorMock, visit(testing::A<const AST::Int32LiteralExpression&>()))
        .WillByDefault(testing::Return(testing::ByMove(Result<llvm::Value*>::Ok(nullptr))));

    Result<llvm::Value*> result = literal.generate(visitorMock);

    EXPECT_TRUE(result.isSuccess);
    EXPECT_THAT(result.data, testing::IsNull());
}

TEST(GenerateLlvmValue, ReturnsResultFromVisitorForBinaryExpression) {
    AstVisitorMock visitorMock;

    auto identifierLeft = new AST::VariableIdentifier("left var");
    auto identifierRight = new AST::VariableIdentifier("right var");
    AST::BinaryExpression binary(AST::BinaryExpressionType::Add, identifierLeft, identifierRight);

    ON_CALL(visitorMock, visit(testing::A<const AST::BinaryExpression&>()))
        .WillByDefault(testing::Return(testing::ByMove(Result<llvm::Value*>::Ok(nullptr))));

    Result<llvm::Value*> result = binary.generate(visitorMock);

    EXPECT_TRUE(result.isSuccess);
    EXPECT_THAT(result.data, testing::IsNull());
}

