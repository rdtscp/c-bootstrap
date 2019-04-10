#include "gtest/gtest.h"

#include "AST.h"

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

TEST(ASTTest, ClassTypeComparison) {
  const atl::shared_ptr<Identifier> classOneIdent(new Identifier("ClassOne"));
  const ClassType classOne(classOneIdent);
  ASSERT_EQ(classOne, classOne);

  const atl::shared_ptr<Identifier> classTwoIdent(new Identifier("ClassTwo"));
  const ClassType classTwo(classTwoIdent);
  ASSERT_NE(classOne, classTwo);
}

TEST(ASTTest, ClassTypeDeclComparisons) {
  const atl::shared_ptr<Identifier> classOneIdent(new Identifier("ClassOne"));
  const atl::shared_ptr<ClassType> classOneType(new ClassType(classOneIdent));

  const ClassTypeDecl classOne(classOneType, {});

  ASSERT_EQ(classOne, ClassTypeDecl(classOne));

  const atl::shared_ptr<Identifier> classTwoIdent(new Identifier("ClassTwo"));
  const atl::shared_ptr<ClassType> classTwoType(new ClassType(classTwoIdent));

  const ClassTypeDecl classTwo(classTwoType, {});

  ASSERT_NE(classOne, classTwo);
}

TEST(ASTTest, FunDeclComparisons) {
  const atl::set<FunDecl::FunModifiers> funDeclOneModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclOneParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("paramOne"))))};
  const atl::shared_ptr<Identifier> funDeclOneIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclOneType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclOne(funDeclOneModifiers, funDeclOneIdent,
                           funDeclOneParams, funDeclOneType);

  const atl::set<FunDecl::FunModifiers> funDeclTwoModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclTwoParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("differentName"))))};
  const atl::shared_ptr<Identifier> funDeclTwoIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclTwoType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclTwo(funDeclTwoModifiers, funDeclTwoIdent,
                           funDeclTwoParams, funDeclTwoType);

  ASSERT_EQ(funDeclOne, funDeclTwo);

  const atl::set<FunDecl::FunModifiers> funDeclThreeModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclThreeParams{
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("differentType"))))};
  const atl::shared_ptr<Identifier> funDeclThreeIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclThreeType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclThree(funDeclThreeModifiers, funDeclThreeIdent,
                             funDeclThreeParams, funDeclThreeType);

  ASSERT_NE(funDeclTwo, funDeclThree);
}

TEST(ASTTest, FunDefComparisons) {
  const atl::set<FunDecl::FunModifiers> funDefOneModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefOneParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("paramOne"))))};
  const atl::shared_ptr<Identifier> funDefOneIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefOneType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefOneBlock(new Block({}));
  const FunDef funDefOne(funDefOneModifiers, funDefOneIdent, funDefOneParams,
                         funDefOneType, funDefOneBlock);

  const atl::set<FunDecl::FunModifiers> funDefTwoModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefTwoParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("differentName"))))};
  const atl::shared_ptr<Identifier> funDefTwoIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefTwoType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefTwoBlock(new Block({}));

  const FunDef funDefTwo(funDefTwoModifiers, funDefTwoIdent, funDefTwoParams,
                         funDefTwoType, funDefTwoBlock);

  ASSERT_EQ(funDefOne, funDefTwo);

  const atl::set<FunDecl::FunModifiers> funDefThreeModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefThreeParams{
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("differentType"))))};
  const atl::shared_ptr<Identifier> funDefThreeIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefThreeType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefThreeBlock(new Block({}));

  const FunDef funDefThree(funDefThreeModifiers, funDefThreeIdent,
                           funDefThreeParams, funDefThreeType,
                           funDefThreeBlock);

  ASSERT_NE(funDefTwo, funDefThree);
}

TEST(ASTTest, FunDeclFunDefComparisons) {
  const atl::set<FunDecl::FunModifiers> funDeclOneModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclOneParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("paramOne"))))};
  const atl::shared_ptr<Identifier> funDeclOneIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclOneType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclOne(funDeclOneModifiers, funDeclOneIdent,
                           funDeclOneParams, funDeclOneType);

  const atl::set<FunDecl::FunModifiers> funDeclTwoModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclTwoParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("differentName"))))};
  const atl::shared_ptr<Identifier> funDeclTwoIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclTwoType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclTwo(funDeclTwoModifiers, funDeclTwoIdent,
                           funDeclTwoParams, funDeclTwoType);

  ASSERT_EQ(funDeclOne, funDeclTwo);

  const atl::set<FunDecl::FunModifiers> funDeclThreeModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDeclThreeParams{
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("differentType"))))};
  const atl::shared_ptr<Identifier> funDeclThreeIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDeclThreeType(
      new BaseType(PrimitiveType::VOID));
  const FunDecl funDeclThree(funDeclThreeModifiers, funDeclThreeIdent,
                             funDeclThreeParams, funDeclThreeType);

  ASSERT_NE(funDeclTwo, funDeclThree);

  const atl::set<FunDecl::FunModifiers> funDefOneModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefOneParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("paramOne"))))};
  const atl::shared_ptr<Identifier> funDefOneIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefOneType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefOneBlock(new Block({}));
  const FunDef funDefOne(funDefOneModifiers, funDefOneIdent, funDefOneParams,
                         funDefOneType, funDefOneBlock);

  const atl::set<FunDecl::FunModifiers> funDefTwoModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefTwoParams = {
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::CHAR)),
          atl::shared_ptr<Identifier>(new Identifier("differentName"))))};
  const atl::shared_ptr<Identifier> funDefTwoIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefTwoType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefTwoBlock(new Block({}));

  const FunDef funDefTwo(funDefTwoModifiers, funDefTwoIdent, funDefTwoParams,
                         funDefTwoType, funDefTwoBlock);

  ASSERT_EQ(funDefOne, funDefTwo);

  const atl::set<FunDecl::FunModifiers> funDefThreeModifiers;
  const atl::vector<atl::shared_ptr<VarDecl>> funDefThreeParams{
      atl::shared_ptr<VarDecl>(new VarDecl(
          atl::shared_ptr<BaseType>(new BaseType(PrimitiveType::INT)),
          atl::shared_ptr<Identifier>(new Identifier("differentType"))))};
  const atl::shared_ptr<Identifier> funDefThreeIdent(new Identifier("myFunc"));
  const atl::shared_ptr<BaseType> funDefThreeType(
      new BaseType(PrimitiveType::VOID));
  const atl::shared_ptr<Block> funDefThreeBlock(new Block({}));

  const FunDef funDefThree(funDefThreeModifiers, funDefThreeIdent,
                           funDefThreeParams, funDefThreeType,
                           funDefThreeBlock);

  ASSERT_NE(funDefTwo, funDefThree);

  ASSERT_EQ(funDeclOne, funDefOne);
  ASSERT_NE(funDeclOne, funDefThree);
}

TEST(ASTTest, Scope_resolveVarExpr_currScope) {
  atl::shared_ptr<Scope> outerScope(new Scope());
  atl::shared_ptr<Scope> currScope(new Scope());
  currScope->outerScope = outerScope;

  /* Store a variable in the current scope. */
  const atl::shared_ptr<BaseType> varDeclType(new BaseType(PrimitiveType::INT));
  const atl::shared_ptr<Identifier> varDeclIdent(new Identifier("var1"));
  const atl::shared_ptr<VarDecl> varDecl(
      new VarDecl(varDeclType, varDeclIdent));
  currScope->insertDecl(varDecl);

  /* Resolve it */
  const atl::shared_ptr<Identifier> searchIdent(new Identifier("var1"));
  atl::shared_ptr<VarDecl> resolvedVarDecl =
      currScope->resolveVarExpr(searchIdent);
  ASSERT_EQ(*varDecl, *resolvedVarDecl);
}

TEST(ASTTest, Scope_resolveVarExpr_outerScope) {
  atl::shared_ptr<Scope> outerScope(new Scope());
  atl::shared_ptr<Scope> currScope(new Scope());
  currScope->outerScope = outerScope;

  /* Store a variable in the current scope. */
  const atl::shared_ptr<BaseType> varDeclType(new BaseType(PrimitiveType::INT));
  const atl::shared_ptr<Identifier> varDeclIdent(new Identifier("var1"));
  const atl::shared_ptr<VarDecl> varDecl(
      new VarDecl(varDeclType, varDeclIdent));
  outerScope->insertDecl(varDecl);

  /* Resolve it */
  const atl::shared_ptr<Identifier> searchIdent(new Identifier("var1"));
  atl::shared_ptr<VarDecl> resolvedVarDecl =
      currScope->resolveVarExpr(searchIdent);
  ASSERT_EQ(*varDecl, *resolvedVarDecl);
}

TEST(ASTTest, Scope_resolveFunCall_innerScope) {
  atl::shared_ptr<Scope> outerScope(new Scope());
  atl::shared_ptr<Scope> currScope(new Scope());
  currScope->outerScope = outerScope;

  /* Store a function in the current scope. */
  const atl::shared_ptr<FunDecl> funDeclOne(
      new FunDecl(atl::set<FunDecl::FunModifiers>(),
                  atl::make_shared<Identifier>(Identifier("myFunc")),
                  {atl::make_shared<VarDecl>(VarDecl(
                      atl::make_shared<BaseType>(BaseType(PrimitiveType::CHAR)),
                      atl::make_shared<Identifier>(Identifier("paramOne"))))},
                  atl::make_shared<BaseType>(BaseType(PrimitiveType::VOID))));

  currScope->insertDecl(funDeclOne);

  /* Resolve it */
  const atl::string funSignature = "myFunc(char)";
  atl::shared_ptr<FunDecl> resolvedFunDecl =
      currScope->resolveFunCall(funSignature);
  ASSERT_EQ(*funDeclOne, *resolvedFunDecl);
}

TEST(ASTTest, ClassTypeDecl_resolveVarExpr) {
  /* Store a class with a member variable in the current scope. */
  const atl::shared_ptr<ClassTypeDecl> classTypeDecl(new ClassTypeDecl(
      atl::make_shared<ClassType>(
          ClassType(atl::make_shared<Identifier>(Identifier("MyClass")))),
      {atl::make_shared<VarDecl>(
          VarDecl(atl::make_shared<BaseType>(BaseType(PrimitiveType::INT)),
                  atl::make_shared<Identifier>(Identifier("memberVar"))))}));

  /* Resolve it */
  const atl::shared_ptr<Identifier> searchIdent(new Identifier("memberVar"));
  atl::shared_ptr<VarDecl> resolvedVarDecl =
      classTypeDecl->resolveVarExpr(searchIdent);
  ASSERT_EQ(resolvedVarDecl.get(), classTypeDecl->classDecls[0].get());
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