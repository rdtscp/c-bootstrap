#include "../include/Token.h"
#include "../include/Position.h"

ACC::Token::Token()
    : tokenClass(ACC::Token::TokenClass::INVALID), position(Position(-1, -1)),
      data("") {}

ACC::Token::Token(ACC::Token::TokenClass type, int lineNum, int colNum,
                  std::string data)
    : tokenClass(type), position(Position(lineNum, colNum)), data(data) {}

ACC::Token::Token(const ACC::Token &t)
    : tokenClass(t.tokenClass), position(t.position), data(t.data) {}

ACC::Token ACC::Token::operator=(Token t) {
  return t;
}