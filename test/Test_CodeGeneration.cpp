#include "atl/include/string.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "passes/DotGraph.h"
#include "passes/SemanticAnalysis.h"

#include "Lexer.h"
#include "LinkerBuilder.h"
#include "Parser.h"
#include "Preprocessor.h"
#include "Scanner.h"
#include "targets/GenerateX64.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_CodeGeneration/";

TEST(Test_CodeGeneration, Allocations) {
  const atl::string filepath = test_prefix + "Allocations/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "Allocations/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

// TEST(Test_CodeGeneration, BinOp) {
//   const atl::string filepath = test_prefix + "BinOp/test.cpp";
//   const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
//   ACC::Preprocessor preprocessor(src, {});
//   ACC::Scanner scanner(preprocessor.getSource());
//   ACC::Lexer lexer(scanner);
//   ACC::Parser parser(lexer);
//   atl::shared_ptr<Program> progAST = parser.getAST();

//   SemanticAnalysis nameAnalysis(progAST);
//   nameAnalysis.run();
//   ASSERT_EQ(0, nameAnalysis.errorCount);

//   GenerateX64 x64Generator(progAST);
//   const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
//   const atl::string binary_name = test_prefix + "BinOp/binary";
//   LinkerBuilder linkAndBuilder(assembly, binary_name);
//   const atl::string binary = linkAndBuilder.linkAndBuild();
//   ASSERT_EQ(system(binary.c_str()), 0);
// }

TEST(Test_CodeGeneration, Dereference) {
  const atl::string filepath = test_prefix + "Dereference/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "Dereference/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, FunCall) {
  const atl::string filepath = test_prefix + "FunCall/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "FunCall/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, MemberAccesses) {
  const atl::string filepath = test_prefix + "MemberAccesses/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "MemberAccesses/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, MemberCall) {
  const atl::string filepath = test_prefix + "MemberCall/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "MemberCall/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, SubscriptClass) {
  const atl::string filepath = test_prefix + "SubscriptClass/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "SubscriptClass/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, SubscriptPtr) {
  const atl::string filepath = test_prefix + "SubscriptPtr/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "SubscriptPtr/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST(Test_CodeGeneration, X64_SimpleFuncs) {
  const atl::string filepath = test_prefix + "X64_SimpleFuncs/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  GenerateX64 x64Generator(progAST);
  const atl::shared_ptr<SourceMemHandler> assembly = x64Generator.run();
  const atl::string binary_name = test_prefix + "X64_SimpleFuncs/binary";
  LinkerBuilder linkAndBuilder(assembly, binary_name);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
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