//
// Created by Bob Fang on 3/27/24.
//

#ifndef LOX_SCANNER_H
#define LOX_SCANNER_H

#include <string>
#include <vector>

#include "Lox.h"
#include "Token.h"

namespace Lox {
class Scanner {
  std::string source;
  std::vector<Token> tokens;

  int start = 0;
  int current = 0;
  int line = 1;

  bool isAtEnd() { return current >= source.size(); }

  char advance() {
    current++;
    return source[current - 1];
  }

  void addToken(TokenType type) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, line);
  }

  void addToken(TokenType type, std::any literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line);
  }

  bool match(char expected) {
    if (isAtEnd())
      return false;
    if (source[current] != expected)
      return false;

    current++;
    return true;
  }

  char peek() {
    if (isAtEnd())
      return '\0';
    return source[current];
  }

  void string() {
    while (peek() != '"' && !isAtEnd()) {
      if (peek() == '\n')
        line++;
      advance();
    }

    if (isAtEnd()) {
      Lox::error(line, "Unterminated string.");
      return;
    }

    // The closing ".
    advance();

    // Trim the surrounding quotes.
    std::string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
  }

  bool isDigit(char c) { return c >= '0' && c <= '9'; }

  char peekNext() {
    if (current + 1 >= source.size())
      return '\0';
    return source[current + 1];
  }

  void number() {
    while (isDigit(peek()))
      advance();

    // Look for a fractional part.
    if (peek() == '.' && isDigit(peekNext())) {
      // Consume the "."
      advance();

      while (isDigit(peek()))
        advance();
    }

    addToken(TokenType::NUMBER,
             std::stod(source.substr(start, current - start)));
  }

  bool isAlpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
  }

  bool isAlphaNumeric(char c) { return isAlpha(c) || isDigit(c); }

  void identifier() {
    while (isAlphaNumeric(peek()))
      advance();

    // See if the identifier is a reserved word.
    std::string text = source.substr(start, current - start);
    TokenType type = TokenType::IDENTIFIER;
    if (text == "and")
      type = TokenType::AND;
    else if (text == "class")
      type = TokenType::CLASS;
    else if (text == "else")
      type = TokenType::ELSE;
    else if (text == "false")
      type = TokenType::FALSE;
    else if (text == "for")
      type = TokenType::FOR;
    else if (text == "fun")
      type = TokenType::FUN;
    else if (text == "if")
      type = TokenType::IF;
    else if (text == "nil")
      type = TokenType::NIL;
    else if (text == "or")
      type = TokenType::OR;
    else if (text == "print")
      type = TokenType::PRINT;
    else if (text == "return")
      type = TokenType::RETURN;
    else if (text == "super")
      type = TokenType::SUPER;
    else if (text == "this")
      type = TokenType::THIS;
    else if (text == "true")
      type = TokenType::TRUE;
    else if (text == "var")
      type = TokenType::VAR;
    else if (text == "while")
      type = TokenType::WHILE;

    addToken(type);
  }

  void scanToken() {
    char c = advance();
    switch (c) {
    case '(':
      addToken(TokenType::LEFT_PAREN);
      break;
    case ')':
      addToken(TokenType::RIGHT_PAREN);
      break;
    case '{':
      addToken(TokenType::LEFT_BRACE);
      break;
    case '}':
      addToken(TokenType::RIGHT_BRACE);
      break;
    case ',':
      addToken(TokenType::COMMA);
      break;
    case '.':
      addToken(TokenType::DOT);
      break;
    case '-':
      addToken(TokenType::MINUS);
      break;
    case '+':
      addToken(TokenType::PLUS);
      break;
    case ';':
      addToken(TokenType::SEMICOLON);
      break;
    case '*':
      addToken(TokenType::STAR);
      break;
    case '!':
      addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
      break;
    case '=':
      addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
      break;
    case '<':
      addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
      break;
    case '>':
      addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
      break;
    case '/':
      if (match('/')) {
        // A comment goes until the end of the line.
        while (peek() != '\n' && !isAtEnd())
          advance();
      } else {
        addToken(TokenType::SLASH);
      }
      break;
    case ' ':
    case '\r':
    case '\t':
      // Ignore whitespace.
      break;
    case '\n':
      line++;
    //                break;
    case '"':
      string();
      break;
    default:
      if (isDigit(c)) {
        number();
      } else if (isAlpha(c)) {
        identifier();
      } else {
        Lox::error(line, "Unexpected character.");
      }
      break;
    }
  }

public:
  explicit Scanner(std::string source) : source(std::move(source)) {}

  std::vector<Token> scanTokens() {
    while (!isAtEnd()) {
      // We are at the beginning of the next lexeme.
      start = current;
      scanToken();
    }
    tokens.emplace_back(TokenType::EoF, "", line);
    return tokens;
  }
};
} // namespace Lox

#endif // LOX_SCANNER_H
