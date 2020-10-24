#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "passes/DotGraph.h"
#include "passes/SemanticAnalysis.h"

#include "TestPath.h"

const atl::string tests_prefix = test_root + "Test_ASTAnalysis/";

class Test_ASTAnalysis : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_ASTAnalysis() {}

  // If the constructor and destructor are not enough for setting up
  // and cleaning up each test, you can define the following methods:
  virtual void SetUp() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = tests_prefix + test_name + "/test.cpp";
  }

  virtual void TearDown() { t_source_file = ""; }
};

using namespace ACC;

// TODO: Test accessing namespace'd classes.

TEST_F(Test_ASTAnalysis, Allocations) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, AssignOverload) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);

  ASSERT_EQ(progAST->decls.size(), 6u);

  ASSERT_EQ(progAST->decls[5]->astClass(), "FunDef");
  const atl::shared_ptr<FunDef> func =
      atl::static_pointer_cast<FunDef>(progAST->decls[5]);
  ASSERT_EQ(func->funBlock->stmts.size(), 3u);
  ASSERT_EQ(func->funBlock->stmts[1]->astClass(), "VarDef");
  const atl::shared_ptr<VarDef> vardef =
      atl::static_pointer_cast<VarDef>(func->funBlock->stmts[1]);

  ASSERT_EQ(vardef->varValue->astClass(), "ConstructorCall");
}

TEST_F(Test_ASTAnalysis, AmbiguousIdentifier) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, CallWithReferences) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, ClassCallsItsMethod) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, ClassCallsItsOwnCtor) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, ClassCallsItsOwnCtorNamespaced) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  try {
    semanticAnalysis.run();
    FAIL() << "Expected ACC::Error";
  } catch (const ACC::Error &err) {
    const atl::string &primary_error = semanticAnalysis.errors[0];
    const int newline_idx = primary_error.find('\t');
    ASSERT_NE(newline_idx, -1);
    const atl::string reason = &(primary_error.c_str()[newline_idx + 1]);
    ASSERT_EQ(reason,
              "Attempted to use a Class that was not declared: atl::string");
  }
}

TEST_F(Test_ASTAnalysis, ClassCallsStatic) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, DestructorMembers) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, DotGraph) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST, "/dev/null");
  dotGraph.print();
}

TEST_F(Test_ASTAnalysis, DuplicateFunction) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, DuplicateVariable) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, FunModifiers) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, FunctionOverloading) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, MemberAccesses) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, MemberCalls) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, MemberVar) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, NameAnalysis) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, NamespaceClass) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, NamespaceFunction) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, NamespacePopulation) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);

  // Assert AST has been modified with namespacing included.
  ASSERT_EQ(progAST->decls[0]->astClass(), "Namespace");
  const atl::shared_ptr<Namespace> nspace =
      atl::static_pointer_cast<Namespace>(progAST->decls[0]);

  ASSERT_EQ(nspace->namespaceDecls[1]->astClass(), "FunDef");
  const atl::shared_ptr<FunDef> getClassFunDef =
      atl::static_pointer_cast<FunDef>(nspace->namespaceDecls[1]);

  ASSERT_EQ(getClassFunDef->funType->astClass(), "ClassType");
  const atl::shared_ptr<ClassType> retType =
      atl::static_pointer_cast<ClassType>(getClassFunDef->funType);

  const atl::shared_ptr<ClassTypeDef> classTypeDef =
      retType->typeDefinition.lock();
  ASSERT_NE(classTypeDef, nullptr);
}

TEST_F(Test_ASTAnalysis, NoMainFunc) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, ReferenceClassBinOp) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, StringClass) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, SubscriptResolution) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, TypedefComparisons) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, TypeDefReturnType) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_EQ(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, UndefinedClass) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, UndefinedFunction) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, UndefinedVariable) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}

TEST_F(Test_ASTAnalysis, UseBeforeDecl) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis semanticAnalysis(progAST);
  semanticAnalysis.run();
  semanticAnalysis.printErrors();
  ASSERT_NE(0, semanticAnalysis.errorCount);
}
