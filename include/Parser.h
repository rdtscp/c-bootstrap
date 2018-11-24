#pragma once

#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "AST.h"
#include "Lexer.h"
#include "Token.h"

namespace ACC {

class Parser {

public:
  Parser(const Lexer &lexer);

  /* Parses the Tokens into an AST with root node Program. */
  Program parse();

private:
  Token currToken;
  Lexer lexer;
  std::vector<Token> tokenBuffer;

  /* ---- Token Operations ---- */

  /* Check if we can expect the provided Token to be next. */
  bool accept(Token::TokenClass expected);

  /* Check if we can expect one of the provided Tokens to be next. */
  bool accept(std::vector<Token::TokenClass> expected);

  /* Expect the next Token to be of a certain type. Throws if not. */
  Token expect(Token::TokenClass expected);

  /* Expect the next Token to be one of a list of types. Throws if not. */
  Token expect(std::vector<Token::TokenClass> expected);

  /* Looks ahead 'i' number of Tokens. */
  Token lookAhead(int i);

  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* ---- Look Ahead ---- */

  bool acceptDecl();

  bool acceptFunDecl();
  bool acceptStructTypeDecl();
  bool acceptVarDecl();

  std::pair<bool, int> acceptType(int startAhead=0);
  bool acceptStructType();

  bool acceptParam();
  bool acceptStmt();
  bool acceptBlock();
  bool acceptWhile();
  bool acceptIf();
  bool acceptReturn();
  bool acceptAssign();

  bool acceptExpr();

  /* ---- Parsing ---- */

  Program parseProgram();

  void parseInclude();
  std::shared_ptr<Decl> parseDecl();

  std::shared_ptr<StructTypeDecl> parseStructTypeDecl();
  std::shared_ptr<VarDecl> parseVarDecl();
  std::shared_ptr<FunDecl> parseFunDecl();
  std::shared_ptr<VarDecl> parseParam();

  std::shared_ptr<Type> parseType();
  std::shared_ptr<StructType> parseStructType();

  std::shared_ptr<Block> parseBlock();

  std::shared_ptr<Stmt> parseStmt();
  std::shared_ptr<While> parseWhile();
  std::shared_ptr<If> parseIf();
  std::shared_ptr<Return> parseReturn();
  std::shared_ptr<Assign> parseAssign();
  
  std::shared_ptr<Expr> parseExpr();
  std::shared_ptr<Expr> parseExpr2();
  std::shared_ptr<Expr> parseExpr3();
  std::shared_ptr<Expr> parseExpr4();
  std::shared_ptr<Expr> parseExpr5();
  std::shared_ptr<Expr> parseExpr6();
  std::shared_ptr<Expr> parseExpr7();
  std::shared_ptr<Expr> parseExpr8();
  std::shared_ptr<Expr> parseExpr9();


  /* ---- Helpers ---- */

  /* Converts an Token to an Type */
  std::shared_ptr<BaseType> tokenToType(const Token::TokenClass &tc);
};

}; // namespace ACC

#endif