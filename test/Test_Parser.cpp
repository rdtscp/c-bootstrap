#include "TestPath.h"
#include "gtest/gtest.h"

#include "AST.h"
#include "Error.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "SourceHandler.h"
#include "SourceToken.h"

class Test_Parser : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_Parser() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = test_root + "Test_Parser/" + test_name + "/test.cpp";
  }
};

using namespace ACC;

TEST_F(Test_Parser, BinOp) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::set<FunDecl::FunModifiers> isAlphaModifiers;
  isAlphaModifiers.insert(FunDecl::FunModifiers::STATIC);
  atl::shared_ptr<Program> expected(new Program(
      {atl::shared_ptr<FunDef>(new FunDef(
           isAlphaModifiers,
           atl::shared_ptr<Identifier>(new Identifier("isalpha")),
           {atl::shared_ptr<VarDecl>(new VarDecl(
               atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
               atl::shared_ptr<Identifier>(new Identifier("c"))))},
           atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::BOOL)),
           atl::shared_ptr<Block>(new Block({atl::shared_ptr<
               Return>(new Return(atl::shared_ptr<ParenthExpr>(
               new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                   atl::shared_ptr<ParenthExpr>(
                       new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                           atl::shared_ptr<ParenthExpr>(
                               new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                                   atl::shared_ptr<CharLiteral>(
                                       new CharLiteral("a")),
                                   Op::LE,
                                   atl::shared_ptr<ParenthExpr>(new ParenthExpr(
                                       atl::shared_ptr<VarExpr>(new VarExpr(
                                           atl::shared_ptr<Identifier>(
                                               new Identifier("c")))))))))),
                           Op::AND,
                           atl::shared_ptr<ParenthExpr>(
                               new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                                   atl::shared_ptr<ParenthExpr>(new ParenthExpr(
                                       atl::shared_ptr<VarExpr>(new VarExpr(
                                           atl::shared_ptr<Identifier>(
                                               new Identifier("c")))))),
                                   Op::LE,
                                   atl::shared_ptr<CharLiteral>(
                                       new CharLiteral("z")))))))))),
                   Op::OR,
                   atl::shared_ptr<ParenthExpr>(
                       new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                           atl::shared_ptr<ParenthExpr>(
                               new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                                   atl::shared_ptr<CharLiteral>(
                                       new CharLiteral("A")),
                                   Op::LE,
                                   atl::shared_ptr<ParenthExpr>(new ParenthExpr(
                                       atl::shared_ptr<VarExpr>(new VarExpr(
                                           atl::shared_ptr<Identifier>(
                                               new Identifier("c")))))))))),
                           Op::AND,
                           atl::shared_ptr<ParenthExpr>(
                               new ParenthExpr(atl::shared_ptr<BinOp>(new BinOp(
                                   atl::shared_ptr<ParenthExpr>(new ParenthExpr(
                                       atl::shared_ptr<VarExpr>(new VarExpr(
                                           atl::shared_ptr<Identifier>(
                                               new Identifier("c")))))),
                                   Op::LE,
                                   atl::shared_ptr<CharLiteral>(
                                       new CharLiteral("Z"))))))))))))))))})))),
       atl::shared_ptr<FunDef>(new FunDef(
           atl::set<FunDecl::FunModifiers>(),
           atl::shared_ptr<Identifier>(new Identifier("main")), {},
           atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
           atl::shared_ptr<Block>(new Block(
               {atl::shared_ptr<VarDef>(new VarDef(
                    atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                    atl::shared_ptr<Identifier>(new Identifier("y")),
                    atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
                atl::shared_ptr<VarDecl>(new VarDecl(
                    atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                    atl::shared_ptr<Identifier>(new Identifier("x")))),
                atl::shared_ptr<Assign>(new Assign(
                    atl::shared_ptr<VarExpr>(new VarExpr(
                        atl::shared_ptr<Identifier>(new Identifier("x")))),
                    atl::shared_ptr<BinOp>(new BinOp(
                        atl::shared_ptr<IntLiteral>(new IntLiteral("1")),
                        Op::ADD,
                        atl::shared_ptr<IntLiteral>(new IntLiteral("2")))))),
                atl::shared_ptr<Return>(new Return(atl::shared_ptr<VarExpr>(
                    new VarExpr(atl::shared_ptr<Identifier>(
                        new Identifier("x"))))))}))))}));

  ASSERT_EQ(*actual, *expected);
}

TEST_F(Test_Parser, ClassDefinition) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  ASSERT_TRUE(true);
}

TEST_F(Test_Parser, ComplexBinOp) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  /* @TODO Test AST */
}

TEST_F(Test_Parser, ComplexParamTypes) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::shared_ptr<BaseType> t1(new BaseType(PrimitiveType::CHAR));
  t1->typeModifiers.insert(Type::Modifiers::CONST);
  atl::shared_ptr<PointerType> t2(new PointerType(t1));
  t2->typeModifiers.insert(Type::Modifiers::CONST);
  atl::shared_ptr<ReferenceType> paramType(new ReferenceType(t2));
  paramType->typeModifiers.insert(Type::Modifiers::CONST);

  atl::shared_ptr<Program> expect(new Program({
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("swap")),
          {atl::shared_ptr<VarDecl>(
               new VarDecl(atl::shared_ptr<ReferenceType>(
                               new ReferenceType(atl::shared_ptr<PointerType>(
                                   new PointerType(atl::shared_ptr<BaseType>(
                                       new BaseType(PrimitiveType::CHAR)))))),
                           atl::shared_ptr<Identifier>(new Identifier("t1")))),
           atl::shared_ptr<VarDecl>(
               new VarDecl(atl::shared_ptr<ReferenceType>(
                               new ReferenceType(atl::shared_ptr<PointerType>(
                                   new PointerType(atl::shared_ptr<BaseType>(
                                       new BaseType(PrimitiveType::CHAR)))))),
                           atl::shared_ptr<Identifier>(new Identifier("t2"))))},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)),
          atl::shared_ptr<Block>(new Block(
              {atl::shared_ptr<VarDef>(new VarDef(
                   atl::shared_ptr<PointerType>(
                       new PointerType(atl::shared_ptr<BaseType>(
                           new BaseType(PrimitiveType::CHAR)))),
                   atl::shared_ptr<Identifier>(new Identifier("temp")),
                   atl::shared_ptr<VarExpr>(new VarExpr(
                       atl::shared_ptr<Identifier>(new Identifier("t1")))))),
               atl::shared_ptr<Assign>(new Assign(
                   atl::shared_ptr<VarExpr>(new VarExpr(
                       atl::shared_ptr<Identifier>(new Identifier("t1")))),
                   atl::shared_ptr<VarExpr>(new VarExpr(
                       atl::shared_ptr<Identifier>(new Identifier("t2")))))),
               atl::shared_ptr<Assign>(new Assign(
                   atl::shared_ptr<VarExpr>(new VarExpr(
                       atl::shared_ptr<Identifier>(new Identifier("t2")))),
                   atl::shared_ptr<VarExpr>(
                       new VarExpr(atl::shared_ptr<Identifier>(
                           new Identifier("temp"))))))})))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("swap")),
          {

              atl::shared_ptr<VarDecl>(new VarDecl(
                  paramType,
                  atl::shared_ptr<Identifier>(new Identifier("t1"))))},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)),
          atl::shared_ptr<Block>(new Block({})))),
  }));

  ASSERT_EQ(*actual, *expect);
}

TEST_F(Test_Parser, FunDecl) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  const unsigned int expected_size = 2;
  ASSERT_EQ(actual->decls.size(), expected_size);
  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("myGlobalInt")))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")),
          {atl::shared_ptr<VarDecl>(new VarDecl(
               atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
               atl::shared_ptr<Identifier>(new Identifier("argc")))),
           atl::shared_ptr<VarDecl>(new VarDecl(
               atl::shared_ptr<PointerType>(
                   new PointerType(atl::shared_ptr<BaseType>(
                       new BaseType(PrimitiveType::CHAR)))),
               atl::shared_ptr<Identifier>(new Identifier("argv"))))},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Block>(new Block({}))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST_F(Test_Parser, InvalidSignatureFunDef) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  try {
    parser.getAST();
  } catch (ACC::Error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected a ACC::Error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected a ACC::Error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST_F(Test_Parser, SubscriptOp) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::shared_ptr<Program> expected =
      atl::shared_ptr<Program>(new Program({atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")), {},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Block>(new Block(
              {atl::shared_ptr<VarDef>(new VarDef(
                   atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                   atl::shared_ptr<Identifier>(new Identifier("y")),
                   atl::shared_ptr<SubscriptOp>(new SubscriptOp(
                       atl::shared_ptr<VarExpr>(new VarExpr(
                           atl::shared_ptr<Identifier>(new Identifier("x")))),
                       atl::shared_ptr<IntLiteral>(new IntLiteral("4")))))),
               atl::shared_ptr<Return>(new Return(
                   atl::shared_ptr<IntLiteral>(new IntLiteral("1"))))}))))}));

  ASSERT_EQ(*actual, *expected);
}

TEST_F(Test_Parser, TemplateDef) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  const unsigned int expected_size = 2;
  ASSERT_EQ(actual->decls.size(), expected_size);
  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::shared_ptr<TemplateDef>(new TemplateDef(
          {atl::shared_ptr<Identifier>(new Identifier("T"))},
          atl::shared_ptr<FunDef>(new FunDef(
              atl::set<FunDecl::FunModifiers>(),
              atl::shared_ptr<Identifier>(new Identifier("swap")),
              {atl::shared_ptr<VarDecl>(new VarDecl(
                   atl::shared_ptr<ReferenceType>(new ReferenceType(
                       atl::shared_ptr<ClassType>(new ClassType(
                           atl::shared_ptr<Identifier>(new Identifier("T")))))),
                   atl::shared_ptr<Identifier>(new Identifier("lhs")))),
               atl::shared_ptr<VarDecl>(new VarDecl(
                   atl::shared_ptr<ReferenceType>(new ReferenceType(
                       atl::shared_ptr<ClassType>(new ClassType(
                           atl::shared_ptr<Identifier>(new Identifier("T")))))),
                   atl::shared_ptr<Identifier>(new Identifier("rhs"))))},
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)),
              atl::shared_ptr<Block>(new Block(
                  {atl::shared_ptr<VarDef>(new VarDef(
                       atl::shared_ptr<ClassType>(new ClassType(
                           atl::shared_ptr<Identifier>(new Identifier("T")))),
                       atl::shared_ptr<Identifier>(new Identifier("temp")),
                       atl::shared_ptr<VarExpr>(
                           new VarExpr(atl::shared_ptr<Identifier>(
                               new Identifier("lhs")))))),
                   atl::shared_ptr<Assign>(new Assign(
                       atl::shared_ptr<VarExpr>(new VarExpr(
                           atl::shared_ptr<Identifier>(new Identifier("lhs")))),
                       atl::shared_ptr<VarExpr>(
                           new VarExpr(atl::shared_ptr<Identifier>(
                               new Identifier("rhs")))))),
                   atl::shared_ptr<Assign>(new Assign(
                       atl::shared_ptr<VarExpr>(new VarExpr(
                           atl::shared_ptr<Identifier>(new Identifier("rhs")))),
                       atl::shared_ptr<VarExpr>(
                           new VarExpr(atl::shared_ptr<Identifier>(
                               new Identifier("temp"))))))})))))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")),
          {atl::shared_ptr<VarDecl>(new VarDecl(
               atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
               atl::shared_ptr<Identifier>(new Identifier("argc")))),
           atl::shared_ptr<VarDecl>(new VarDecl(
               atl::shared_ptr<PointerType>(
                   new PointerType(atl::shared_ptr<PointerType>(
                       new PointerType(atl::shared_ptr<BaseType>(
                           new BaseType(PrimitiveType::CHAR)))))),
               atl::shared_ptr<Identifier>(new Identifier("argv"))))},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Block>(new Block(
              {atl::shared_ptr<VarDef>(new VarDef(
                   atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                   atl::shared_ptr<Identifier>(new Identifier("a")),
                   atl::shared_ptr<IntLiteral>(new IntLiteral("10")))),
               atl::shared_ptr<VarDef>(new VarDef(
                   atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                   atl::shared_ptr<Identifier>(new Identifier("b")),
                   atl::shared_ptr<IntLiteral>(new IntLiteral("0")))),
               atl::shared_ptr<FunCall>(new FunCall(
                   atl::shared_ptr<Identifier>(new Identifier("swap")),
                   {atl::shared_ptr<VarExpr>(new VarExpr(
                        atl::shared_ptr<Identifier>(new Identifier("a")))),
                    atl::shared_ptr<VarExpr>(new VarExpr(
                        atl::shared_ptr<Identifier>(new Identifier("b"))))})),
               atl::shared_ptr<Return>(new Return(atl::shared_ptr<VarExpr>(
                   new VarExpr(atl::shared_ptr<Identifier>(
                       new Identifier("a"))))))}))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]) << "decls[" << i << "]";
}

TEST_F(Test_Parser, VarDecls) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));

  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("myInt")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("myChar")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(new PointerType(
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)))),
          atl::shared_ptr<Identifier>(new Identifier("myIntPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(new PointerType(
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)))),
          atl::shared_ptr<Identifier>(new Identifier("myCharPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(new PointerType(
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)))),
          atl::shared_ptr<Identifier>(new Identifier("myVoidPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(
              new PointerType(atl::shared_ptr<PointerType>(
                  new PointerType(atl::shared_ptr<BaseType>(
                      new BaseType(PrimitiveType::INT)))))),
          atl::shared_ptr<Identifier>(new Identifier("myIntPtrPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(
              new PointerType(atl::shared_ptr<PointerType>(
                  new PointerType(atl::shared_ptr<BaseType>(
                      new BaseType(PrimitiveType::CHAR)))))),
          atl::shared_ptr<Identifier>(new Identifier("myCharPtrPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<PointerType>(
              new PointerType(atl::shared_ptr<PointerType>(
                  new PointerType(atl::shared_ptr<BaseType>(
                      new BaseType(PrimitiveType::VOID)))))),
          atl::shared_ptr<Identifier>(new Identifier("myVoidPtrPtr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<ArrayType>(new ArrayType(
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
              atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
          atl::shared_ptr<Identifier>(new Identifier("myIntArr")))),
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<ArrayType>(new ArrayType(
              atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
              atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
          atl::shared_ptr<Identifier>(new Identifier("myCharArr"))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}
