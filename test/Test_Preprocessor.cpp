#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "../include/Parser.h"
#include "../include/Preprocessor.h"
#include "../include/SourceCode.h"
#include "../include/passes/DotGraph.h"
#include "../include/passes/NameAnalysis.h"
#include "../include/passes/TypeAnalysis.h"

using namespace ACC;

// std::string test_prefix =
// "/Users/alexanderwilson/Documents/GitHub/c-bootstrap/test/tests/";
std::string test_prefix = "../../test/tests/";

TEST(PreprocessorTest, IncludeWorks) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, NestedIncludes) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog3.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();

  ASSERT_EQ(progAST->decls.size(), 3);
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, IfNDef) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog2.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();

  ASSERT_EQ(progAST->decls.size(), 2);
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, PragmaOnce) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog4.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();

  ASSERT_EQ(progAST->decls.size(), 3);
  ASSERT_EQ(0, nameAnalysis.errorCount);
}

TEST(PreprocessorTest, NestedIfDefs) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog5.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  ASSERT_EQ(progAST->decls.size(), 3);
  ASSERT_EQ(progAST->decls[0]->getIdentifier(), "funB0");
  ASSERT_EQ(progAST->decls[1]->getIdentifier(), "funC0");
  ASSERT_EQ(progAST->decls[2]->getIdentifier(), "main");
}

TEST(PreprocessorTest, IfElifElse) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog6.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  ASSERT_EQ(progAST->decls.size(), 3);
  ASSERT_EQ(progAST->decls[0]->getIdentifier(), "funA3");
  ASSERT_EQ(progAST->decls[1]->getIdentifier(), "funB5");
  ASSERT_EQ(progAST->decls[2]->getIdentifier(), "main");
}

TEST(PreprocessorTest, IfElifElse2) {
  Preprocessor preprocessor(test_prefix + "preprocessor/prog7.c");
  SourceCode src = preprocessor.getSource();

  Parser parser(src);
  std::shared_ptr<Program> progAST = parser.getAST();

  ASSERT_EQ(progAST->decls.size(), 1);
  ASSERT_EQ(progAST->decls[0]->getIdentifier(), "two");
}

TEST(PreprocessorTest, InvalidComments) {
  Preprocessor preprocessor(test_prefix + "lexer/errors.c");
  try {
    SourceCode src = preprocessor.getSource();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (std::exception const &err) {
    std::cout << "Expected a std::runtime_error, but got:" << err.what()
              << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, no exception thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(PreprocessorTest, InvalidIncludes) {
  Preprocessor preprocessor(test_prefix + "lexer/inclood.c");
  try {
    SourceCode src = preprocessor.getSource();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (std::exception const &err) {
    std::cout << "Expected a std::runtime_error, but got:" << err.what()
              << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, no exception thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

TEST(PreprocessorTest, InvalidIncludes2) {
  Preprocessor preprocessor(test_prefix + "lexer/include_error.c");
  try {
    SourceCode src = preprocessor.getSource();
  } catch (std::runtime_error const &err) {
    ASSERT_TRUE(true);
    return;
  } catch (std::exception const &err) {
    std::cout << "Expected a std::runtime_error, but got:" << err.what()
              << std::endl;
    ASSERT_TRUE(false);
    return;
  }
  std::cout << "Expected a std::runtime_error, no exception thrown."
            << std::endl;
  ASSERT_TRUE(false);
}

// The fixture for testing class Project1. From google test primer.
class Test_Preprocessor : public ::testing::Test {
protected:
  Test_Preprocessor() {
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