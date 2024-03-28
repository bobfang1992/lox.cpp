//
// Created by Bob Fang on 3/28/24.
//

#ifndef LOX_EXPR_H
#define LOX_EXPR_H

#include <memory>
#include <utility>
#include <vector>

#include "Token.h"

namespace Lox {

struct Expr {
  virtual ~Expr() = default;
};

struct Assign : public Expr {
  Token name;
  std::shared_ptr<Expr> value;

  Assign(Token name, std::shared_ptr<Expr> value)
      : name(std::move(name)), value(std::move(value)) {}
};

struct Binary : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Binary(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
};

struct Call : public Expr {
  std::shared_ptr<Expr> callee;
  Token paren;
  std::vector<std::shared_ptr<Expr>> arguments;

  Call(std::shared_ptr<Expr> callee, Token paren,
       std::vector<std::shared_ptr<Expr>> arguments)
      : callee(std::move(callee)), paren(std::move(paren)),
        arguments(std::move(arguments)) {}
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
};

struct Literal : public Expr {
  std::any value;

  explicit Literal(std::any value) : value(std::move(value)) {}
};

struct Logical : public Expr {
  std::shared_ptr<Expr> left;
  Token op;
  std::shared_ptr<Expr> right;

  Logical(std::shared_ptr<Expr> left, Token op, std::shared_ptr<Expr> right)
      : left(std::move(left)), op(std::move(op)), right(std::move(right)) {}
};

struct Set : public Expr {
  std::shared_ptr<Expr> object;
  Token name;
  std::shared_ptr<Expr> value;

  Set(std::shared_ptr<Expr> object, Token name, std::shared_ptr<Expr> value)
      : object(std::move(object)), name(std::move(name)),
        value(std::move(value)) {}
};

struct Super : public Expr {
  Token keyword;
  Token method;

  Super(Token keyword, Token method)
      : keyword(std::move(keyword)), method(std::move(method)) {}
};

struct This : public Expr {
  Token keyword;

  explicit This(Token keyword) : keyword(std::move(keyword)) {}
};

struct Unary : public Expr {
  Token op;
  std::shared_ptr<Expr> right;

  Unary(Token op, std::shared_ptr<Expr> right)
      : op(std::move(op)), right(std::move(right)) {}
};

struct Variable : public Expr {
  Token name;

  explicit Variable(Token name) : name(std::move(name)) {}
};

} // namespace Lox

#endif // LOX_EXPR_H
