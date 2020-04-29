#include "atl/include/string.h"

#include "gtest/gtest.h"

#include "Error.h"
#include "Lexer.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "passes/DotGraph.h"
#include "passes/SemanticAnalysis.h"

using namespace ACC;

// atl::string test_prefix = 
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/"
// "test/tests/Test_ASTAnalysis/";
atl::string test_prefix = "../../test/tests/Test_ASTAnalysis/";

// TODO: Test accessing namespace'd classes.

TEST(Test_ASTAnalysis, AmbiguousIdentifier) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "AmbiguousIdentifier/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "CallWithReferences/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ClassCallsItsMethod/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ClassCallsItsOwnCtor/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ClassCallsItsOwnCtorNamespaced/test.cpp");
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

TEST(Test_ASTAnalysis, DotGraph) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "DotGraph/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  Parser parser(lexer);

  atl::shared_ptr<Program> progAST = parser.getAST();

  DotGraph dotGraph(progAST, "/dev/null");
  dotGraph.print();
}

TEST(Test_ASTAnalysis, DuplicateFunction) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "DuplicateFunction/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "DuplicateVariable/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "FunModifiers/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "FunctionOverloading/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "MemberAccesses/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "MemberCalls/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NameAnalysis/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NamespaceClass/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NamespaceFunction/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "NoMainFunc/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "ReferenceClassBinOp/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "StringClass/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "SubscriptResolution/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TypedefComparisons/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "TypeDefReturnType/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "UndefinedClass/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "UndefinedFunction/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "UndefinedVariable/test.cpp");
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
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "UseBeforeDecl/test.cpp");
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