#include <vector>

#include "gtest/gtest.h"

#include "../include/Parser.h"
#include "../include/SourceHandler.h"
#include "../include/SourceToken.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
atl::string test_prefix = "../../test/tests/";


TEST(ParserTest, AllTokens) {
  const atl::string filepath = test_prefix + "lexer/alltokens.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  try {
    parser.getAST();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected a std::runtime_error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected a std::runtime_error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST(ParserTest, InvalidIdentifier) {
  const atl::string filepath = test_prefix + "lexer/errors.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  try {
    ACC::Lexer lexer(scanner);
    ACC::Parser parser(lexer);
    parser.getAST();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected a std::runtime_error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected a std::runtime_error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
}

TEST(ParserTest, NestedComments) {
  const atl::string filepath = test_prefix + "lexer/nested_comments.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> p = parser.getAST();

  ASSERT_TRUE(true);
}

TEST(ParserTest, StructDecl) {
  const atl::string filepath = test_prefix + "parser/structdecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> actual = parser.getAST();

  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<StructTypeDecl>(StructTypeDecl(
          std::make_shared<StructType>(StructType("FooStruct")),
          {std::make_shared<VarDecl>(
              VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      "fooInt"))})),
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<StructType>(StructType("FooStruct")),
                  "myFooStruct"))};

  int actualSize = actual->decls.size();
  int expectSize = expectedDecls.size();
  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, VarDecls) {
  const atl::string filepath = test_prefix + "parser/vardecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> actual = parser.getAST();

  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<StructTypeDecl>(StructTypeDecl(
          std::make_shared<StructType>(StructType("FooStruct")),
          {std::make_shared<VarDecl>(
              VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      "fooInt"))})),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<BaseType>(BaseType(PrimitiveType::INT)), "myInt")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)), "myChar")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(PointerType(
              std::make_shared<BaseType>(BaseType(PrimitiveType::INT)))),
          "myIntPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(PointerType(
              std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
          "myCharPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(PointerType(
              std::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))),
          "myVoidPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(
              PointerType(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::INT)))))),
          "myIntPtrPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(
              PointerType(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))))),
          "myCharPtrPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<PointerType>(
              PointerType(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))))),
          "myVoidPtrPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<ArrayType>(ArrayType(
              std::make_shared<BaseType>(BaseType(PrimitiveType::INT)), "5")),
          "myIntArr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<ArrayType>(ArrayType(
              std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)), "5")),
          "myCharArr")),
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<StructType>(StructType("FooStruct")),
                  "myFooStruct")),
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<PointerType>(PointerType(
                      std::make_shared<StructType>(StructType("FooStruct")))),
                  "myFooStructPtr")),
      std::make_shared<VarDecl>(VarDecl(
          std::make_shared<ArrayType>(ArrayType(
              std::make_shared<StructType>(StructType("FooStruct")), "5")),
          "myFooStructArr"))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, FunDecl) {
  const atl::string filepath = test_prefix + "parser/fundecl.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> actual = parser.getAST();
  ASSERT_EQ(actual->decls.size(), 2);
  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  "myGlobalInt")),
      std::shared_ptr<FunDef>(new FunDef(
          std::make_shared<Block>(Block({})), "main",
          {std::make_shared<VarDecl>(
               VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                       "argc")),
           std::make_shared<VarDecl>(VarDecl(
               std::make_shared<PointerType>(PointerType(
                   std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
               "argv"))},
          std::make_shared<BaseType>(BaseType(PrimitiveType::INT))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, BinOp) {
  const atl::string filepath = test_prefix + "parser/binop.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> actual = parser.getAST();
  ASSERT_EQ(actual->decls.size(), 1);
  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::shared_ptr<FunDef>(new FunDef(
          std::make_shared<Block>(Block(
              {std::make_shared<VarDecl>(VarDecl(
                   std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   "x")),
               std::make_shared<Assign>(Assign(
                   std::make_shared<VarExpr>(VarExpr("x")),
                   std::make_shared<BinOp>(BinOp(
                       std::make_shared<IntLiteral>(IntLiteral("1")), Op::ADD,
                       std::make_shared<IntLiteral>(IntLiteral("2")))))),
                       std::make_shared<Return>(Return(std::make_shared<VarExpr>(VarExpr("x"))))})),
          "main", {},
          std::make_shared<BaseType>(BaseType(PrimitiveType::INT))))};

  ASSERT_EQ(actual->decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual->decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, ComplexBinOp) {
  const atl::string filepath = test_prefix + "parser/fundecls.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  std::shared_ptr<Program> actual = parser.getAST();
  /* @TODO Test AST */
}

TEST(ParserTest, InvalidSignatureFunDef) {
  const atl::string filepath = test_prefix + "parser/invalidfundef.c";
  const SourceHandler src(SourceHandler::Type::FILEPATH, filepath);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  try {
    parser.getAST();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    printf("Expected a std::runtime_error\n");
    ASSERT_TRUE(false);
    return;
  }
  printf("Expected a std::runtime_error, but no exception was thrown.\n");
  ASSERT_TRUE(false);
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