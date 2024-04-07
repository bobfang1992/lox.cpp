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
    virtual ~Visitor() = default;
    virtual std::any visitAssign(const Assign &expr) = 0;
    virtual std::any visitBinary(const Binary &expr) = 0;
    virtual std::any visitCall(const Call &expr) = 0;
    virtual std::any visitGet(const Get &expr) = 0;
    virtual std::any visitGrouping(const Grouping &expr) = 0;
    virtual std::any visitLiteral(const Literal &expr) = 0;
    virtual std::any visitLogical(const Logical &expr) = 0;
    virtual std::any visitSet(const Set &expr) = 0;
    virtual std::any visitSuper(const Super &expr) = 0;
    virtual std::any visitThis(const This &expr) = 0;
    virtual std::any visitUnary(const Unary &expr) = 0;
    virtual std::any visitVariable(const Variable &expr) = 0;
  };

  virtual std::any accept(Visitor &visitor) const = 0;
};

struct Assign : public Expr {
  Token name;
  std::shared_ptr<Expr> value;

  Assign(Token name, std::shared_ptr<Expr> value)
      : name(std::move(name)), value(std::move(value)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitAssign(*this);
  }
};

struct Binary : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitBinary(*this);
  }
};

struct Call : public Expr {
  std::shared_ptr<Expr> callee;
  Token paren;
  std::vector<std::shared_ptr<Expr>> arguments;

  Call(std::shared_ptr<Expr> callee, Token paren,
       std::vector<std::shared_ptr<Expr>> arguments)
      : callee(std::move(callee)), paren(std::move(paren)),
        arguments(std::move(arguments)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitCall(*this);
  }
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

  std::any accept(Visitor &visitor) const override {
    return visitor.visitGrouping(*this);
  }
};

struct Literal : public Expr {
  std::variant<double, std::string, bool, nullptr_t> value;
  explicit Literal(std::variant<double, std::string, bool, nullptr_t> value)
      : value(std::move(value)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitLiteral(*this);
  }
};

struct Logical : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitLogical(*this);
  }
};

struct Set : public Expr {
  std::shared_ptr<Expr> object;
  Token name;
  std::shared_ptr<Expr> value;

  Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
      : object(std::move(object)), name(std::move(name)),
        value(std::move(value)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitSet(*this);
  }
};

struct Super : public Expr {
  Token keyword;
  Token method;

  Super(Token keyword, Token method)
      : keyword(std::move(keyword)), method(std::move(method)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitSuper(*this);
  }
};

struct This : public Expr {
  Token keyword;

  explicit This(Token keyword) : keyword(std::move(keyword)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitThis(*this);
  }
};

struct Unary : public Expr {
  Token op;
  std::shared_ptr<Expr> right;

  Unary(Token op, std::shared_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitUnary(*this);
  }
};

struct Variable : public Expr {
  Token name;

  explicit Variable(Token name) : name(std::move(name)) {}

  std::any accept(Visitor &visitor) const override {
    return visitor.visitVariable(*this);
  }
};

} // namespace Lox

#endif // LOX_EXPR_H
