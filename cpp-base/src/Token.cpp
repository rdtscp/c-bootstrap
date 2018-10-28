#include "../include/Token.h"
#include "../include/Position.h"

using namespace ACC;

Token::Token()
    : tokenClass(Token::TokenClass::INVALID), position(Position(-1, -1)),
      data("") {}

Token::Token(Token::TokenClass type, int lineNum, int colNum,
                  std::string data)
    : tokenClass(type), position(Position(lineNum, colNum)), data(data) {}
