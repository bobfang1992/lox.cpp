//
// Created by Bob Fang on 4/7/24.
//

#ifndef LOX_STATEMENT_H
#define LOX_STATEMENT_H

#include <any>
#include <vector>

#include "Token.h"
#include "Expr.h"

namespace Lox {
struct Statement;
struct Block;
struct Class;
struct Expression;
struct Function;
struct If;
struct Print;
struct Return;
struct Var;
struct While;

struct Statement {
  virtual ~Statement() = default;

  struct Visitor {
    virtual ~Visitor() = default;
    virtual std::any visitBlock(const Block &stmt) = 0;
    virtual std::any visitClass(const Class &stmt) = 0;
    virtual std::any visitExpression(const Expression &stmt) = 0;
    virtual std::any visitFunction(const Function &stmt) = 0;
    virtual std::any visitIf(const If &stmt) = 0;
    virtual std::any visitPrint(const Print &stmt) = 0;
    virtual std::any visitReturn(const Return &stmt) = 0;
    virtual std::any visitVar(const Var &stmt) = 0;
    virtual std::any visitWhile(const While &stmt) = 0;
  };

  virtual std::any accept(Visitor &visitor) = 0;
};

struct Block : public Statement {
  std::vector<std::shared_ptr<Statement>> statements;

  explicit Block(std::vector<std::shared_ptr<Statement>> statements)
      : statements(std::move(statements)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitBlock(*this);
  }
};

struct Class : public Statement {
  Token name;
  std::shared_ptr<Variable> superclass;
  std::vector<std::shared_ptr<Function>> methods;

  Class(Token name, std::shared_ptr<Variable> superclass,
        std::vector<std::shared_ptr<Function>> methods)
      : name(std::move(name)), superclass(std::move(superclass)),
        methods(std::move(methods)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitClass(*this);
  }
};

struct Expression : public Statement {
  std::shared_ptr<Expr> expression;

  explicit Expression(std::shared_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitExpression(*this);
  }
};

struct Function : public Statement {
  Token name;
  std::vector<Token> params;
  std::vector<std::shared_ptr<Statement>> body;

  Function(Token name, std::vector<Token> params,
           std::vector<std::shared_ptr<Statement>> body)
      : name(std::move(name)), params(std::move(params)),
        body(std::move(body)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitFunction(*this);
  }
};

struct If : public Statement {
  std::shared_ptr<Expr> condition;
  std::shared_ptr<Statement> thenBranch;
  std::shared_ptr<Statement> elseBranch;

  If(std::shared_ptr<Expr> condition, std::shared_ptr<Statement> thenBranch,
     std::shared_ptr<Statement> elseBranch)
      : condition(std::move(condition)), thenBranch(std::move(thenBranch)),
        elseBranch(std::move(elseBranch)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitIf(*this);
  }
};

struct Print : public Statement {
  std::shared_ptr<Expr> expression;

  explicit Print(std::shared_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitPrint(*this);
  }
};

struct Return : public Statement {
  Token keyword;
  std::shared_ptr<Expr> value;

  Return(Token keyword, std::shared_ptr<Expr> value)
      : keyword(std::move(keyword)), value(std::move(value)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitReturn(*this);
  }
};

struct Var : public Statement {
  Token name;
  std::shared_ptr<Expr> initializer;

  Var(Token name, std::shared_ptr<Expr> initializer)
      : name(std::move(name)), initializer(std::move(initializer)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitVar(*this);
  }
};

struct While : public Statement {
  std::shared_ptr<Expr> condition;
  std::shared_ptr<Statement> body;

  While(std::shared_ptr<Expr> condition, std::shared_ptr<Statement> body)
      : condition(std::move(condition)), body(std::move(body)) {}

  std::any accept(Visitor &visitor) override {
    return visitor.visitWhile(*this);
  }
};

} // namespace Lox

#endif // LOX_STATEMENT_H
