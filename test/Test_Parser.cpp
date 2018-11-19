#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Lexer.h"
#include "../include/Parser.h"
#include "../include/Scanner.h"
#include "../include/Token.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/cpp-base/test/tests/";
std::string test_prefix = "../../test/tests/";

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

TEST(ParserTest, VarDecls) {
  Scanner scanner(test_prefix + "parser/vardecl.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program actual = parser.parse();
  ASSERT_EQ(actual.funDecls.size(), 0);
  ASSERT_EQ(actual.structTypeDecls.size(), 1);
  ASSERT_EQ(actual.varDecls.size(), 13);

  ASSERT_EQ(actual.structTypeDecls[0].structType->identifier, "FooStruct");
  ASSERT_EQ(actual.structTypeDecls[0].varDecls.size(), 1);
  ASSERT_EQ(actual.structTypeDecls[0].varDecls[0]->identifer, "fooInt");

  ASSERT_TRUE(
      instanceOf<BaseType>(*actual.structTypeDecls[0].varDecls[0]->type.get()));
  BaseType *bt = static_cast<BaseType *>(
      actual.structTypeDecls[0].varDecls[0]->type.get());
  ASSERT_EQ(PrimitiveType::INT, bt->primitiveType);
  // ASSERT_EQ(static_cast<BaseType>(*actual.structTypeDecls[0].varDecls[0].type.get()).primitiveType,
  // PrimitiveType::INT);

  Type *struct0Field0Type = actual.structTypeDecls[0].varDecls[0]->type.get();

  std::vector<VarDecl> expectedVarDecls = {
      VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
              std::string("myInt")),
      VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
              std::string("myChar")),

      VarDecl(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::INT)))),
              std::string("myIntPtr")),
      VarDecl(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
              std::string("myCharPtr")),
      VarDecl(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::VOID)))),
              std::string("myVoidPtr")),

      VarDecl(std::make_shared<PointerType>(std::make_shared<PointerType>(
                  PointerType(std::make_shared<BaseType>(
                      BaseType(PrimitiveType::INT))))),
              std::string("myIntPtrPtr")),
      VarDecl(std::make_shared<PointerType>(std::make_shared<PointerType>(
                  PointerType(std::make_shared<BaseType>(
                      BaseType(PrimitiveType::CHAR))))),
              std::string("myCharPtrPtr")),
      VarDecl(std::make_shared<PointerType>(std::make_shared<PointerType>(
                  PointerType(std::make_shared<BaseType>(
                      BaseType(PrimitiveType::VOID))))),
              std::string("myVoidPtrPtr")),

      VarDecl(
          std::make_shared<ArrayType>(
              std::make_shared<BaseType>(BaseType(PrimitiveType::INT)), "5"),
          std::string("myIntArr")),
      VarDecl(
          std::make_shared<ArrayType>(
              std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)), "5"),
          std::string("myCharArr")),

      VarDecl(std::make_shared<StructType>(StructType("myFooStruct")),
              std::string("myFooStruct")),
      VarDecl(std::make_shared<PointerType>(
                  std::make_shared<StructType>(StructType("myFooStruct"))),
              std::string("myFooStruct")),
      VarDecl(std::make_shared<ArrayType>(
                  std::make_shared<StructType>(StructType("myFooStruct")), "5"),
              std::string("myFooStruct"))};

  std::vector<VarDecl> actualVarDecls;
  for (const VarDecl &varDecl : actual.varDecls) {
    actualVarDecls.push_back(varDecl);
  }

  ASSERT_TRUE(true);
}

TEST(ParserTest, FunDecl) {
  Scanner scanner(test_prefix + "parser/fundecl.c");
  Lexer lexer(scanner);
  Parser parser(lexer);

  Program prog = parser.parse();
  ASSERT_EQ(prog.varDecls.size(), 1);
  ASSERT_EQ(prog.funDecls.size(), 1);

  std::vector<std::shared_ptr<Stmt>> blockStmts;
  std::shared_ptr<Block> expectedBlock(new Block(blockStmts));
  std::vector<std::shared_ptr<VarDecl>> expectedParams = {
      std::make_shared<VarDecl>(VarDecl(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
              std::string("argc"))),
      std::make_shared<VarDecl>(VarDecl(std::make_shared<PointerType>(PointerType(
                  std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)))),
              std::string("argv")))};

  FunDecl actual = prog.funDecls[0];
  FunDecl expected(std::make_shared<Block>(Block(blockStmts)), std::string("main"), expectedParams,
                   std::make_shared<BaseType>(BaseType(PrimitiveType::INT)));

  ASSERT_TRUE(actual == expected);
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