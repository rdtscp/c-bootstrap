#include "gtest/gtest.h"

#include "AST.h"
#include "Error.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "SourceHandler.h"
#include "SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/"
//   "test/tests/Test_Parser/";
atl::string test_prefix = "../../test/tests/Test_Parser/";

TEST(Test_Parser, BinOp) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "BinOp/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::set<FunDecl::FunModifiers> isAlphaModifiers;
  isAlphaModifiers.insert(FunDecl::FunModifiers::STATIC);
  atl::shared_ptr<Program> expected(new Program({
      atl::shared_ptr<FunDef>(new FunDef(
          isAlphaModifiers,
          atl::shared_ptr<Identifier>(new Identifier("isalpha")),
          {atl::make_shared<VarDecl>(
              VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                      atl::shared_ptr<Identifier>(new Identifier("c"))))},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::BOOL)),
          atl::make_shared<Block>(Block({atl::make_shared<
              Return>(Return(atl::make_shared<ParenthExpr>(
              ParenthExpr(atl::make_shared<BinOp>(BinOp(
                  atl::make_shared<ParenthExpr>(ParenthExpr(atl::make_shared<
                                                            BinOp>(BinOp(
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<CharLiteral>(CharLiteral("a")),
                              Op::LE,
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::shared_ptr<Identifier>(
                                          new Identifier("c")))))))))),
                      Op::AND,
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(
                              BinOp(atl::make_shared<ParenthExpr>(
                                        ParenthExpr(atl::make_shared<VarExpr>(
                                            VarExpr(atl::shared_ptr<Identifier>(
                                                new Identifier("c")))))),
                                    Op::LE,
                                    atl::make_shared<CharLiteral>(
                                        CharLiteral("z")))))))))),
                  Op::OR,
                  atl::make_shared<
                      ParenthExpr>(ParenthExpr(atl::make_shared<BinOp>(BinOp(
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(BinOp(
                              atl::make_shared<CharLiteral>(CharLiteral("A")),
                              Op::LE,
                              atl::make_shared<ParenthExpr>(
                                  ParenthExpr(atl::make_shared<VarExpr>(
                                      VarExpr(atl::shared_ptr<Identifier>(
                                          new Identifier("c")))))))))),
                      Op::AND,
                      atl::make_shared<ParenthExpr>(
                          ParenthExpr(atl::make_shared<BinOp>(
                              BinOp(atl::make_shared<ParenthExpr>(
                                        ParenthExpr(atl::make_shared<VarExpr>(
                                            VarExpr(atl::shared_ptr<Identifier>(
                                                new Identifier("c")))))),
                                    Op::LE,
                                    atl::make_shared<CharLiteral>(
                                        CharLiteral("Z"))))))))))))))))})))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")), {},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
          atl::make_shared<Block>(Block(
              {atl::make_shared<VarDef>(VarDef(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   atl::shared_ptr<Identifier>(new Identifier("y")),
                   atl::make_shared<IntLiteral>(IntLiteral("5")))),
               atl::make_shared<VarDecl>(VarDecl(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   atl::shared_ptr<Identifier>(new Identifier("x")))),
               atl::make_shared<Assign>(Assign(
                   atl::make_shared<VarExpr>(VarExpr(
                       atl::shared_ptr<Identifier>(new Identifier("x")))),
                   atl::make_shared<BinOp>(BinOp(
                       atl::make_shared<IntLiteral>(IntLiteral("1")), Op::ADD,
                       atl::make_shared<IntLiteral>(IntLiteral("2")))))),
               atl::make_shared<Return>(Return(atl::make_shared<VarExpr>(
                   VarExpr(atl::shared_ptr<Identifier>(
                       new Identifier("x"))))))}))))}));

  ASSERT_EQ(*actual, *expected);
}

TEST(Test_Parser, ClassDefinition) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ClassDefinition/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  ASSERT_TRUE(true);
}

TEST(Test_Parser, ComplexBinOp) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ComplexBinOp/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  /* @TODO Test AST */
}

TEST(Test_Parser, FunDecl) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "FunDecl/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();
  const unsigned int expected_size = 2;
  ASSERT_EQ(actual->decls.size(), expected_size);
  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  atl::shared_ptr<Identifier>(new Identifier("myGlobalInt")))),
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")),
          {atl::make_shared<VarDecl>(
               VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                       atl::shared_ptr<Identifier>(new Identifier("argc")))),
           atl::make_shared<VarDecl>(VarDecl(
               atl::make_shared<PointerType>(PointerType(
                   atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
               atl::shared_ptr<Identifier>(new Identifier("argv"))))},
          atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
          atl::make_shared<Block>(Block({}))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(Test_Parser, InvalidSignatureFunDef) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "InvalidSignatureFunDef/test.cpp");
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

TEST(Test_Parser, SubscriptOp) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "SubscriptOp/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::shared_ptr<Program> expected = atl::shared_ptr<Program>(new Program({
      atl::shared_ptr<FunDef>(new FunDef(
          atl::set<FunDecl::FunModifiers>(),
          atl::shared_ptr<Identifier>(new Identifier("main")),
          {},
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Block>(new Block(
              {
                atl::shared_ptr<VarDef>(new VarDef(
                    atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                    atl::shared_ptr<Identifier>(new Identifier("y")),
                    atl::shared_ptr<SubscriptOp>(new SubscriptOp(
                        atl::shared_ptr<VarExpr>(new VarExpr(
                            atl::shared_ptr<Identifier>(new Identifier("x"))
                        )),
                        atl::shared_ptr<IntLiteral>(new IntLiteral("4"))
                    ))
                )),
                atl::shared_ptr<Return>(new Return(
                     atl::shared_ptr<IntLiteral>(new
                     IntLiteral("1"))))
              }
          ))
      ))
  }));

  ASSERT_EQ(*actual, *expected);
}

TEST(Test_Parser, VarDecls) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "VarDecls/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> actual = parser.getAST();

  atl::vector<atl::shared_ptr<Decl>> expectedDecls = {
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  atl::shared_ptr<Identifier>(new Identifier("myInt")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                  atl::shared_ptr<Identifier>(new Identifier("myChar")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)))),
          atl::shared_ptr<Identifier>(new Identifier("myIntPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
          atl::shared_ptr<Identifier>(new Identifier("myCharPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(PointerType(
              atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))),
          atl::shared_ptr<Identifier>(new Identifier("myVoidPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)))))),
          atl::shared_ptr<Identifier>(new Identifier("myIntPtrPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))))),
          atl::shared_ptr<Identifier>(new Identifier("myCharPtrPtr")))),
      atl::make_shared<VarDecl>(VarDecl(
          atl::make_shared<PointerType>(
              PointerType(atl::make_shared<PointerType>(PointerType(
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))))),
          atl::shared_ptr<Identifier>(new Identifier("myVoidPtrPtr")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<ArrayType>(ArrayType(
                      atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
                  atl::shared_ptr<Identifier>(new Identifier("myIntArr")))),
      atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<ArrayType>(ArrayType(
                      atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                      atl::shared_ptr<IntLiteral>(new IntLiteral("5")))),
                  atl::shared_ptr<Identifier>(new Identifier("myCharArr"))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (unsigned int i = 0; i < expectedDecls.size(); ++i)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

// The fixture for testing class Project1. From google test primer.
class Test_Parser : public ::testing::Test {
protected:
  Test_Parser() {
    // You can do set-up work for each test here.
  }

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    // Code here will be called immediately after the constructor (right
    // before each test).
  }

  virtual void TearDown() {
    // Code here will be called immediately after each test (right
    // before the destructor).
  }
};