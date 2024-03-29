//
// Created by Bob Fang on 3/28/24.
//

#ifndef LOX_EXPR_H
#define LOX_EXPR_H

#include <memory>
#include <utility>
#include <variant>
#include <vector>

#include "Token.h"

namespace Lox {
struct Expr;
struct Assign;
struct Binary;
struct Call;
struct Get;
struct Grouping;
struct Literal;
struct Logical;
struct Set;
struct Super;
struct This;
struct Unary;
struct Variable;

struct Expr {
  virtual ~Expr() = default;

  struct Visitor {
    virtual void visitAssign(const Assign &expr) = 0;
    virtual void visitBinary(const Binary &expr) = 0;
    virtual void visitCall(const Call &expr) = 0;
    virtual void visitGet(const Get &expr) = 0;
    virtual void visitGrouping(const Grouping &expr) = 0;
    virtual void visitLiteral(const Literal &expr) = 0;
    virtual void visitLogical(const Logical &expr) = 0;
    virtual void visitSet(const Set &expr) = 0;
    virtual void visitSuper(const Super &expr) = 0;
    virtual void visitThis(const This &expr) = 0;
    virtual void visitUnary(const Unary &expr) = 0;
    virtual void visitVariable(const Variable &expr) = 0;
  };

  virtual void accept(Visitor &visitor) const = 0;
};

struct Assign : public Expr {
  Token name;
  std::shared_ptr<Expr> value;

  Assign(Token name, std::shared_ptr<Expr> value)
      : name(std::move(name)), value(std::move(value)) {}

  void accept(Visitor &visitor) const override { visitor.visitAssign(*this); }
};

struct Binary : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  void accept(Visitor &visitor) const override { visitor.visitBinary(*this); }
};

struct Call : public Expr {
  std::shared_ptr<Expr> callee;
  Token paren;
  std::vector<std::shared_ptr<Expr>> arguments;

  Call(std::shared_ptr<Expr> callee, Token paren,
       std::vector<std::shared_ptr<Expr>> arguments)
      : callee(std::move(callee)), paren(std::move(paren)),
        arguments(std::move(arguments)) {}

  void accept(Visitor &visitor) const override { visitor.visitCall(*this); }
};

struct Get : public Expr {
  std::shared_ptr<Expr> object;
  Token name;

  Get(std::shared_ptr<Expr> object, Token name)
      : object(std::move(object)), name(std::move(name)) {}
};

struct Grouping : public Expr {
  std::shared_ptr<Expr> expression;

  explicit Grouping(std::shared_ptr<Expr> expression)
      : expression(std::move(expression)) {}

  void accept(Visitor &visitor) const override { visitor.visitGrouping(*this); }
};

struct Literal : public Expr {
  std::variant<double, std::string, bool> value;
  explicit Literal(std::variant<double, std::string, bool> value)
      : value(std::move(value)) {}

  void accept(Visitor &visitor) const override { visitor.visitLiteral(*this); }
};

struct Logical : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  void accept(Visitor &visitor) const override { visitor.visitLogical(*this); }
};

struct Set : public Expr {
  std::shared_ptr<Expr> object;
  Token name;
  std::shared_ptr<Expr> value;

  Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
      : object(std::move(object)), name(std::move(name)),
        value(std::move(value)) {}

  void accept(Visitor &visitor) const override { visitor.visitSet(*this); }
};

struct Super : public Expr {
  Token keyword;
  Token method;

  Super(Token keyword, Token method)
      : keyword(std::move(keyword)), method(std::move(method)) {}

  void accept(Visitor &visitor) const override { visitor.visitSuper(*this); }
};

struct This : public Expr {
  Token keyword;

  explicit This(Token keyword) : keyword(std::move(keyword)) {}

  void accept(Visitor &visitor) const override { visitor.visitThis(*this); }
};

struct Unary : public Expr {
  Token op;
  std::shared_ptr<Expr> right;

  Unary(Token op, std::shared_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  void accept(Visitor &visitor) const override { visitor.visitUnary(*this); }
};

struct Variable : public Expr {
  Token name;

  explicit Variable(Token name) : name(std::move(name)) {}

  void accept(Visitor &visitor) const override { visitor.visitVariable(*this); }
};

} // namespace Lox

#endif // LOX_EXPR_H
