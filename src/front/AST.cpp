#include "front/AST.h"

#include <string>

#include "back/AstVisitor.h"
#include "common/Utils.h"
#include "common/Log.h"

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
}

