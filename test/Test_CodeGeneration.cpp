#include "atl/include/string.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "passes/DotGraph.h"
#include "passes/SemanticAnalysis.h"

#include "Lexer.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "targets/GenerateX64.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_CodeGeneration/";

TEST(Test_CodeGeneration, Dereference) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "Dereference/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "Dereference/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, FunCall) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "FunCall/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "FunCall/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, MemberAccesses) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "MemberAccesses/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "MemberAccesses/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, MemberCall) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "MemberCall/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "MemberCall/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, SubscriptClass) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "SubscriptClass/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "SubscriptClass/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, SubscriptPtr) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "SubscriptPtr/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "SubscriptPtr/test.s");
  x64Generator.run();
}

TEST(Test_CodeGeneration, X64_SimpleFuncs) {
  const SourceHandler src(SourceHandler::Type::FILEPATH,
                          test_prefix + "X64_SimpleFuncs/test.cpp");
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST, test_prefix + "X64_SimpleFuncs/test.s");
  x64Generator.run();
}

// The fixture for testing class Project1. From google test primer.
class Test_CodeGeneration : public ::testing::Test {
protected:
  Test_CodeGeneration() {
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