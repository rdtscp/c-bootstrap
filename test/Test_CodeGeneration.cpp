
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

#include <cstdio>
#include <fstream>

class Test_CodeGeneration : public ::testing::Test {
private:
  bool fileExists(const atl::string &filepath) {
    std::ifstream file(filepath.c_str());
    return file.good();
  }

protected:
  atl::string t_source_file;
  atl::string t_binary_file;

  Test_CodeGeneration() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    const atl::string tests_prefix = test_root + "Test_CodeGeneration/";
    t_source_file = tests_prefix + test_name + "/test.cpp";
    t_binary_file = tests_prefix + test_name + "/binary";
  }

  ~Test_CodeGeneration() {
    if (fileExists(t_binary_file)) {
      std::remove(t_binary_file.c_str());
    }

    t_binary_file = "";
    t_source_file = "";
  }
};

using namespace ACC;

TEST_F(Test_CodeGeneration, AddrOfRef) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, Allocations) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, AssignOverload) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, BinOp) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, Blocks) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, ClassPointer) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, CopyConstructor) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, Dereference) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, Destructor) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, FunCall) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, InitialiserListConstruction) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, MemberAccesses) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, MemberCall) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, NamespacedFunctions) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, PassByReference) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, PointerLooping) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, String) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, StringView) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, SubscriptClass) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, SubscriptPtr) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}

TEST_F(Test_CodeGeneration, X64_SimpleFuncs) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
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
  LinkerBuilder linkAndBuilder(assembly, t_binary_file);
  const atl::string binary = linkAndBuilder.linkAndBuild();
  ASSERT_EQ(system(binary.c_str()), 0);
}
