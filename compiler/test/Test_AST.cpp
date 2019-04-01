#include "gtest/gtest.h"

#include "../include/AST.h"

using namespace ACC;

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
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));
  ArrayType intArray(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));
  ArrayType voidArray(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));

  ArrayType charArray2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));
  ArrayType intArray2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));
  ArrayType voidArray2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)),
      atl::shared_ptr<IntLiteral>(new IntLiteral("5")));

  ASSERT_FALSE(charArray == intArray);
  ASSERT_FALSE(intArray == voidArray);

  ASSERT_TRUE(charArray == charArray2);
  ASSERT_TRUE(intArray == intArray2);
  ASSERT_TRUE(voidArray == voidArray2);
}

TEST(ASTTest, PointerTypeComparisons) {
  PointerType charPtr(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)));
  PointerType intPtr(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)));
  PointerType voidPtr(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)));

  PointerType charPtr2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)));
  PointerType intPtr2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)));
  PointerType voidPtr2(
      atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::VOID)));

  ASSERT_FALSE(charPtr == intPtr);
  ASSERT_FALSE(intPtr == voidPtr);

  ASSERT_TRUE(charPtr == charPtr2);
  ASSERT_TRUE(intPtr == intPtr2);
  ASSERT_TRUE(voidPtr == voidPtr2);
}

TEST(ASTTest, StructTypeComparisons) {
  StructType structType(atl::make_shared<Identifier>(Identifier("struct1")));
  StructType structType2(atl::make_shared<Identifier>(Identifier("struct2")));
  StructType structType3(atl::make_shared<Identifier>(Identifier("struct2")));

  ASSERT_FALSE(structType == structType2);

  ASSERT_TRUE(structType2 == structType3);
}

TEST(ASTTest, VarDeclComparisons) {
  VarDecl vd1(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
              atl::make_shared<Identifier>(Identifier("var1")));
  VarDecl vd2(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
              atl::make_shared<Identifier>(Identifier("var2")));
  VarDecl vd3(atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
              atl::make_shared<Identifier>(Identifier("var2")));

  ASSERT_FALSE(vd1 == vd2);
  ASSERT_TRUE(vd2 == vd3);
}

TEST(ASTTest, FunDeclFunDefComparisons) {
  const atl::set<FunDecl::FunModifiers> funModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> params;

  const atl::shared_ptr<FunDecl> funDeclA(new FunDecl(
      funModifiers, atl::make_shared<Identifier>(Identifier("functionA")), {},
      atl::make_shared<BaseType>(BaseType(PrimitiveType::INT))));

  const atl::shared_ptr<FunDef> funDefA(new FunDef(
      funModifiers, atl::make_shared<Identifier>(Identifier("functionA")), {},
      atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
      atl::make_shared<Block>(Block({}))));

  ASSERT_TRUE(*funDefA == *funDeclA);
  ASSERT_TRUE(*funDeclA == *funDefA);
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