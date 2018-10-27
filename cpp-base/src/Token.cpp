#include "Token.h"
#include "Position.h"

ACC::Token::Token(ACC::Token::TokenClass type, int lineNum, int colNum,
                  std::string data)
    : tokenClass(type), position(Position(lineNum, colNum)), data(data) {}

std::string ACC::Token::toString() {
  if (data == "") {
    return std::string((int)tokenClass + "");
  }
  return std::string((int)tokenClass + "(" + data + ")");
}