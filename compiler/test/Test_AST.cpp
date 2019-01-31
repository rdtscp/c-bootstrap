#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/AST.h"

using namespace ACC;

// atl::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/compiler/test/tests/";
atl::string test_prefix = "../../test/tests/";

TEST(ASTTest, BaseTypeComparisons) {

  BaseType charType(PrimitiveType::CHAR);
  BaseType intType(PrimitiveType::INT);
  BaseType voidType(PrimitiveType::VOID);

  BaseType charType2(PrimitiveType::CHAR);
  BaseType intType2(PrimitiveType::INT);
  BaseType voidType2(PrimitiveType::VOID);

  ASSERT_FALSE(charType == intType);
  ASSERT_FALSE(intType == voidType);

  ASSERT_TRUE(charType == charType2);
  ASSERT_TRUE(intType == intType2);
  ASSERT_TRUE(voidType == voidType2);
}

TEST(ASTTest, ArrayTypeComparisons) {
  ArrayType charArray(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)), "5");
  ArrayType intArray(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)), "5");
  ArrayType voidArray(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)), "5");

  ArrayType charArray2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)), "5");
  ArrayType intArray2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)), "5");
  ArrayType voidArray2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)), "5");

  ASSERT_FALSE(charArray == intArray);
  ASSERT_FALSE(intArray == voidArray);

  ASSERT_TRUE(charArray == charArray2);
  ASSERT_TRUE(intArray == intArray2);
  ASSERT_TRUE(voidArray == voidArray2);
}

TEST(ASTTest, PointerTypeComparisons) {
  PointerType charPtr(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)));
  PointerType intPtr(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)));
  PointerType voidPtr(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)));

  PointerType charPtr2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)));
  PointerType intPtr2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)));
  PointerType voidPtr2(
      std::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)));

  ASSERT_FALSE(charPtr == intPtr);
  ASSERT_FALSE(intPtr == voidPtr);

  ASSERT_TRUE(charPtr == charPtr2);
  ASSERT_TRUE(intPtr == intPtr2);
  ASSERT_TRUE(voidPtr == voidPtr2);
}

TEST(ASTTest, StructTypeComparisons) {
  StructType structType(atl::string("struct1"));
  StructType structType2(atl::string("struct2"));
  StructType structType3(atl::string("struct2"));

  ASSERT_FALSE(structType == structType2);

  ASSERT_TRUE(structType2 == structType3);
}

TEST(ASTTest, VarDeclComparisons) {
  VarDecl vd1(std::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
              atl::string("myInt"));
  VarDecl vd2(std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
              atl::string("myChar"));
  VarDecl vd3(std::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
              atl::string("myChar"));

  ASSERT_FALSE(vd1 == vd2);
  ASSERT_TRUE(vd2 == vd3);
}

// The fixture for testing class Project1. From google test primer.
class Test_AST : public ::testing::Test {
protected:
  Test_AST() {
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