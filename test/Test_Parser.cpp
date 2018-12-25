#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"
#include "../include/Token.h"

using namespace ACC;

std::string test_prefix =
"/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
// std::string test_prefix = "../../test/tests/";

TEST(ParserTest, AllTokens) {
  Scanner scanner(test_prefix + "lexer/alltokens.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  try {
    parser.parse();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    std::cout << "Expected a std::runtime_error" << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, but no exception was thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(ParserTest, InvalidIdentifier) {
  Scanner scanner(test_prefix + "lexer/errors.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  try {
    parser.parse();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (...) {
    std::cout << "Expected a std::runtime_error" << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, but no exception was thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(ParserTest, NestedComments) {
  Scanner scanner(test_prefix + "lexer/nested_comments.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program p = parser.parse();

  ASSERT_TRUE(true);
}

TEST(ParserTest, StructDecl) {
  Scanner scanner(test_prefix + "parser/structdecl.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();

  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<StructTypeDecl>(StructTypeDecl(
          std::make_shared<StructType>(StructType("FooStruct")),
          {std::make_shared<VarDecl>(
              VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                      "fooInt"))})),
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<StructType>(StructType("FooStruct")),
                  "myFooStruct"))};

  int actualSize = actual.decls.size();
  int expectSize = expectedDecls.size();
  ASSERT_EQ(actual.decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual.decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, VarDecls) {
  Scanner scanner(test_prefix + "parser/vardecl.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();

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

  ASSERT_EQ(actual.decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual.decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, FunDecl) {
  Scanner scanner(test_prefix + "parser/fundecl.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();
  ASSERT_EQ(actual.decls.size(), 2);
  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<VarDecl>(
          VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  "myGlobalInt")),
      std::make_shared<FunDecl>(FunDecl(
          std::make_shared<Block>(Block({})), "main",
          {std::make_shared<VarDecl>(
               VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                       "argc")),
           std::make_shared<VarDecl>(VarDecl(
               std::make_shared<PointerType>(PointerType(
                   std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
               "argv"))},
          std::make_shared<BaseType>(BaseType(PrimitiveType::INT))))};

  ASSERT_EQ(actual.decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual.decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, BinOp) {
  Scanner scanner(test_prefix + "parser/binop.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();
  ASSERT_EQ(actual.decls.size(), 1);
  std::vector<std::shared_ptr<Decl>> expectedDecls = {
      std::make_shared<FunDecl>(FunDecl(
          std::make_shared<Block>(Block(
              {std::make_shared<VarDecl>(VarDecl(
                   std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                   "x")),
               std::make_shared<Assign>(Assign(
                   std::make_shared<VarExpr>(VarExpr("x")),
                   std::make_shared<BinOp>(BinOp(
                       std::make_shared<IntLiteral>(IntLiteral("1")), Op::ADD,
                       std::make_shared<IntLiteral>(IntLiteral("2"))))))})),
          "main", {},
          std::make_shared<BaseType>(BaseType(PrimitiveType::INT))))};

  ASSERT_EQ(actual.decls.size(), expectedDecls.size());

  for (int i = 0; i < expectedDecls.size(); i++)
    ASSERT_TRUE(*actual.decls[i] == *expectedDecls[i]);
}

TEST(ParserTest, ComplexBinOp) {
  Scanner scanner(test_prefix + "parser/fundecls.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();
  /* TODO Test AST */
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