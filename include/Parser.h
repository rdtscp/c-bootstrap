#pragma once

#include "atl/include/shared_ptr.h"
#include "atl/include/string.h"
#include "atl/include/vector.h"

#include "AST.h"
#include "Lexer.h"
#include "SourceToken.h"

namespace ACC {

class Parser final {

public:
  Parser(Lexer &lexer);
  Parser(const Parser &rhs) = delete;
  Parser(Parser &&rhs) = delete;
  Parser &operator=(const Parser &rhs) = delete;
  Parser &operator=(Parser &&rhs) = delete;

  /* Parses the Tokens into an AST with root node Program. */
  atl::shared_ptr<Program> getAST();

private:
  Position prevPosition;
  SourceToken currToken;
  Lexer &lexer;
  atl::vector<SourceToken> tokenBuffer;

  /* ---- Token Operations ---- */

  /* Check if we can expect the provided Token to be next. */
  bool accept(SourceToken::Class expected, int offset = 0);

  /* Check if we can expect one of the provided Tokens to be next. */
  bool accept(atl::vector<SourceToken::Class> expected, int offset = 0);

  /* Expect the next Token to be of a certain type. Throws if not. */
  SourceToken expect(SourceToken::Class expected);

  /* Expect the next Token to be one of a list of types. Throws if not. */
  SourceToken expect(atl::vector<SourceToken::Class> expected);

  /* Looks ahead 'i' number of Tokens. */
  SourceToken lookAhead(unsigned int i);

  /* Populates currToken with the next unconsumed Token. */
  void nextToken();

  /* ---- Look Ahead ---- */

  /* -- Decls -- */
  bool acceptAccessModifier(int offset = 0);
  bool acceptClassTypeDecl(int offset = 0);
  bool acceptConstructor(int offset = 0);
  bool acceptDecl(int offset = 0);
  bool acceptDestructor(int offset = 0);
  bool acceptEnumClassTypeDecl(int offset = 0);
  bool acceptFunDecl(int offset = 0);
  bool acceptOpOverload(int offset = 0);
  bool acceptTemplateDef(int offset = 0);
  bool acceptTypeDefDecl(int offset = 0);
  bool acceptVarDecl(int offset = 0);

  /* -- Types -- */
  bool acceptClassType(int offset = 0);
  bool acceptType(int offset = 0);

  /* -- Stmts -- */
  bool acceptAllocation(int offset = 0);
  bool acceptAssign(int offset = 0);
  bool acceptBlock(int offset = 0);
  bool acceptDeletion(int offset = 0);
  bool acceptDoWhile(int offset = 0);
  bool acceptFor(int offset = 0);
  bool acceptIf(int offset = 0);
  bool acceptNamespace(int offset = 0);
  bool acceptReturn(int offset = 0);
  bool acceptStmt(int offset = 0);
  bool acceptThrow(int offset = 0);
  bool acceptWhile(int offset = 0);

  bool acceptExpr(int offset = 0);
  bool acceptObjExpr(int offset = 0);
  bool acceptObjExprOp(int offset = 0);
  bool acceptLitExpr(int offset = 0);

  bool acceptOperatorOverload(int offset = 0);
  bool acceptParam(int offset = 0);

  /* ---- Parsing ---- */

  atl::shared_ptr<Program> parseProgram();

  atl::shared_ptr<Identifier> parseIdentifier();

  /* -- Decls -- */
  atl::shared_ptr<ClassTypeDecl> parseClassTypeDecl();
  atl::shared_ptr<ConstructorDecl> parseConstructor();
  atl::shared_ptr<Decl> parseDecl();
  atl::shared_ptr<DestructorDecl> parseDestructor();
  atl::shared_ptr<EnumClassTypeDecl> parseEnumClassTypeDecl();
  atl::shared_ptr<FunDecl> parseFunDecl();
  atl::shared_ptr<TemplateDef> parseTemplateDef();
  atl::shared_ptr<TypeDefDecl> parseTypeDefDecl();
  atl::shared_ptr<VarDecl> parseVarDecl();

  /* -- Types -- */
  atl::shared_ptr<Type> parseType();

  /* -- Stmts -- */
  atl::shared_ptr<Assign> parseAssign();
  atl::shared_ptr<Block> parseBlock();
  atl::shared_ptr<Deletion> parseDeletion();
  atl::shared_ptr<DoWhile> parseDoWhile();
  atl::shared_ptr<For> parseFor();
  atl::shared_ptr<If> parseIf();
  atl::shared_ptr<Namespace> parseNamespace();
  atl::shared_ptr<Return> parseReturn();
  atl::shared_ptr<Stmt> parseStmt();
  atl::shared_ptr<Throw> parseThrow();
  atl::shared_ptr<While> parseWhile();

  /* -- Exprs -- */
  atl::shared_ptr<Expr> parseExpr();
  atl::shared_ptr<Expr> parseBoolExpr();
  atl::shared_ptr<Expr> parseEqualExpr();
  atl::shared_ptr<Expr> parseCompExpr();
  atl::shared_ptr<Expr> parseAddExpr();
  atl::shared_ptr<Expr> parseMulExpr();
  atl::shared_ptr<Expr> parseUnaryExpr();
  atl::shared_ptr<Expr> parseObjExpr();
  atl::shared_ptr<Expr> parseObjExprOp();
  atl::shared_ptr<FunCall> parseFunCall();
  atl::shared_ptr<Expr> parseLitExpr();
  atl::shared_ptr<StringLiteral> parseStringLiteral();

  atl::string parseOperatorOverload();
  atl::shared_ptr<VarDecl> parseParam();

  /* ---- Helpers ---- */

  /* Converts an Token to an Type */
  atl::shared_ptr<BaseType> tokenToType(const SourceToken::Class &tc);

  template <typename T>
  atl::shared_ptr<T> createNode(T *node) {
    node->position = prevPosition;
    return atl::shared_ptr<T>(node);
  }
};

} // namespace ACC
