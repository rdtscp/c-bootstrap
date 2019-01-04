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
  Parser(Lexer &lexer);

  /* Parses the Tokens into an AST with root node Program. */
  std::shared_ptr<Program> parse();

private:
  Token currToken;
  Lexer &lexer;
  std::vector<Token> tokenBuffer;

  /* ---- Token Operations ---- */

  /* Check if we can expect the provided Token to be next. */
  bool accept(Token::TokenClass expected, int offset = 0);

  /* Check if we can expect one of the provided Tokens to be next. */
  bool accept(std::vector<Token::TokenClass> expected, int offset = 0);

  /* Expect the next Token to be of a certain type. Throws if not. */
  Token expect(Token::TokenClass expected);

  /* Expect the next Token to be one of a list of types. Throws if not. */
  Token expect(std::vector<Token::TokenClass> expected);

  /* Looks ahead 'i' number of Tokens. */
  Token lookAhead(int i);

  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* ---- Look Ahead ---- */

  /* -- Decls -- */
  bool acceptDecl(int offset = 0);
  bool acceptEnumTypeDecl(int offset = 0);
  bool acceptFunDecl(int offset = 0);
  bool acceptStructTypeDecl(int offset = 0);
  bool acceptTypeDefDecl(int offset = 0);
  bool acceptVarDecl(int offset = 0);

  /* -- Types -- */
  bool acceptStructType(int offset = 0);
  bool acceptType(int offset = 0);

  /* -- Stmts -- */
  bool acceptAssign(int offset = 0);
  bool acceptBlock(int offset = 0);
  bool acceptDoWhile(int offset = 0);
  bool acceptIf(int offset = 0);
  bool acceptReturn(int offset = 0);
  bool acceptStmt(int offset = 0);
  bool acceptWhile(int offset = 0);

  bool acceptParam(int offset = 0);

  bool acceptExpr(int offset = 0);

  /* ---- Parsing ---- */

  std::shared_ptr<Program> parseProgram();

  /* -- Decls -- */
  std::shared_ptr<Decl> parseDecl();
  std::shared_ptr<EnumTypeDecl> parseEnumTypeDecl();
  std::shared_ptr<FunDecl> parseFunDecl();
  std::shared_ptr<StructTypeDecl> parseStructTypeDecl();
  std::shared_ptr<TypeDefDecl> parseTypeDefDecl();
  std::shared_ptr<VarDecl> parseVarDecl();

  /* -- Types -- */
  std::shared_ptr<StructType> parseStructType();
  std::shared_ptr<Type> parseType();

  /* -- Stmts -- */
  std::shared_ptr<Assign> parseAssign();
  std::shared_ptr<Block> parseBlock();
  std::shared_ptr<DoWhile> parseDoWhile();
  std::shared_ptr<If> parseIf();
  std::shared_ptr<Return> parseReturn();
  std::shared_ptr<Stmt> parseStmt();
  std::shared_ptr<While> parseWhile();

  std::shared_ptr<VarDecl> parseParam();

  /* -- Exprs -- */
  std::shared_ptr<Expr> parseExpr();
  std::shared_ptr<Expr> parseBoolExpr();
  std::shared_ptr<Expr> parseEqualExpr();
  std::shared_ptr<Expr> parseCompExpr();
  std::shared_ptr<Expr> parseAddExpr();
  std::shared_ptr<Expr> parseMulExpr();
  std::shared_ptr<Expr> parseUnaryExpr();
  std::shared_ptr<Expr> parseObjExpr();
  std::shared_ptr<Expr> parseLitExpr();

  /* ---- Helpers ---- */

  /* Converts an Token to an Type */
  std::shared_ptr<BaseType> tokenToType(const Token::TokenClass &tc);
};

}; // namespace ACC

#endif