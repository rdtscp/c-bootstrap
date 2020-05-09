#include "atl/include/string.h"

#include "gtest/gtest.h"
#include "TestPath.h"

#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "passes/DotGraph.h"
#include "passes/SemanticAnalysis.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_ASTAnalysis/";


// TODO: Test accessing namespace'd classes.

TEST(Test_ASTAnalysis, Allocations) {
  const atl::string filepath = test_prefix + "Allocations/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, AmbiguousIdentifier) {
  const atl::string filepath = test_prefix + "AmbiguousIdentifier/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, CallWithReferences) {
  const atl::string filepath = test_prefix + "CallWithReferences/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, ClassCallsItsMethod) {
  const atl::string filepath = test_prefix + "ClassCallsItsMethod/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, ClassCallsItsOwnCtor) {
  const atl::string filepath = test_prefix + "ClassCallsItsOwnCtor/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, ClassCallsItsOwnCtorNamespaced) {
  const atl::string filepath = test_prefix + "ClassCallsItsOwnCtorNamespaced/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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
    const atl::string reason = &(primary_error.c_str()[newline_idx+1]);
    ASSERT_EQ(reason, "Attempted to use a Class that was not declared: atl::string");
  }
}

TEST(Test_ASTAnalysis, ClassCallsStatic) {
  const atl::string filepath = test_prefix + "ClassCallsStatic/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, DotGraph) {
  const atl::string filepath = test_prefix + "DotGraph/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST, "/dev/null");
  dotGraph.print();
}

TEST(Test_ASTAnalysis, DuplicateFunction) {
  const atl::string filepath = test_prefix + "DuplicateFunction/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, DuplicateVariable) {
  const atl::string filepath = test_prefix + "DuplicateVariable/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, FunModifiers) {
  const atl::string filepath = test_prefix + "FunModifiers/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, FunctionOverloading) {
  const atl::string filepath = test_prefix + "FunctionOverloading/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, MemberAccesses) {
  const atl::string filepath = test_prefix + "MemberAccesses/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, MemberCalls) {
  const atl::string filepath = test_prefix + "MemberCalls/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, NameAnalysis) {
  const atl::string filepath = test_prefix + "NameAnalysis/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, NamespaceClass) {
  const atl::string filepath = test_prefix + "NamespaceClass/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, NamespaceFunction) {
  const atl::string filepath = test_prefix + "NamespaceFunction/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, NoMainFunc) {
  const atl::string filepath = test_prefix + "NoMainFunc/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, ReferenceClassBinOp) {
  const atl::string filepath = test_prefix + "ReferenceClassBinOp/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, StringClass) {
  const atl::string filepath = test_prefix + "StringClass/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, SubscriptResolution) {
  const atl::string filepath = test_prefix + "SubscriptResolution/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, TypedefComparisons) {
  const atl::string filepath = test_prefix + "TypedefComparisons/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, TypeDefReturnType) {
  const atl::string filepath = test_prefix + "TypeDefReturnType/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, UndefinedClass) {
  const atl::string filepath = test_prefix + "UndefinedClass/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, UndefinedFunction) {
  const atl::string filepath = test_prefix + "UndefinedFunction/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, UndefinedVariable) {
  const atl::string filepath = test_prefix + "UndefinedVariable/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(Test_ASTAnalysis, UseBeforeDecl) {
  const atl::string filepath = test_prefix + "UseBeforeDecl/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

// The fixture for testing class Project1. From google test primer.
class Test_ASTAnalysis : public ::testing::Test {
protected:
  Test_ASTAnalysis() {
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