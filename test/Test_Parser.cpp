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
      std::shared_ptr<StructTypeDecl>(new StructTypeDecl(
          std::shared_ptr<StructType>(new StructType("FooStruct")),
          {std::shared_ptr<VarDecl>(new VarDecl(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
              "fooInt"))})),
      std::shared_ptr<VarDecl>(
          new VarDecl(std::shared_ptr<StructType>(new StructType("FooStruct")),
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
      std::shared_ptr<StructTypeDecl>(new StructTypeDecl(
          std::shared_ptr<StructType>(new StructType("FooStruct")),
          {std::shared_ptr<VarDecl>(new VarDecl(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
              "fooInt"))})),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          "myInt")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          "myChar")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<PointerType>(new PointerType(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)))),
          "myIntPtr")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<PointerType>(new PointerType(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)))),
          "myCharPtr")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<PointerType>(new PointerType(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)))),
          "myVoidPtr")),
      std::shared_ptr<VarDecl>(
          new VarDecl(std::shared_ptr<PointerType>(
                          new PointerType(std::shared_ptr<PointerType>(
                              new PointerType(std::shared_ptr<BaseType>(
                                  new BaseType(PrimitiveType::INT)))))),
                      "myIntPtrPtr")),
      std::shared_ptr<VarDecl>(
          new VarDecl(std::shared_ptr<PointerType>(
                          new PointerType(std::shared_ptr<PointerType>(
                              new PointerType(std::shared_ptr<BaseType>(
                                  new BaseType(PrimitiveType::CHAR)))))),
                      "myCharPtrPtr")),
      std::shared_ptr<VarDecl>(
          new VarDecl(std::shared_ptr<PointerType>(
                          new PointerType(std::shared_ptr<PointerType>(
                              new PointerType(std::shared_ptr<BaseType>(
                                  new BaseType(PrimitiveType::VOID)))))),
                      "myVoidPtrPtr")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<ArrayType>(new ArrayType(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
              "5")),
          "myIntArr")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<ArrayType>(new ArrayType(
              std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
              "5")),
          "myCharArr")),
      std::shared_ptr<VarDecl>(
          new VarDecl(std::shared_ptr<StructType>(new StructType("FooStruct")),
                      "myFooStruct")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<PointerType>(new PointerType(
              std::shared_ptr<StructType>(new StructType("FooStruct")))),
          "myFooStructPtr")),
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<ArrayType>(new ArrayType(
              std::shared_ptr<StructType>(new StructType("FooStruct")), "5")),
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
      std::shared_ptr<VarDecl>(new VarDecl(
          std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          "myGlobalInt")),
      std::shared_ptr<FunDecl>(new FunDecl(
          std::shared_ptr<Block>(new Block({})), "main",
          {std::shared_ptr<VarDecl>(new VarDecl(
               std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
               "argc")),
           std::shared_ptr<VarDecl>(
               new VarDecl(std::shared_ptr<PointerType>(
                               new PointerType(std::shared_ptr<BaseType>(
                                   new BaseType(PrimitiveType::CHAR)))),
                           "argv"))},
          std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT))))};

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
      std::shared_ptr<FunDecl>(new FunDecl(
          std::shared_ptr<Block>(new Block(
              {std::shared_ptr<VarDecl>(new VarDecl(
                   std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
                   "x")),
               std::shared_ptr<Assign>(new Assign(
                   std::shared_ptr<VarExpr>(new VarExpr("x")),
                   std::shared_ptr<BinOp>(new BinOp(
                       std::shared_ptr<IntLiteral>(new IntLiteral("1")),
                       Op::ADD,
                       std::shared_ptr<IntLiteral>(new IntLiteral("2"))))))})),
          "main", {},
          std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT))))};

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