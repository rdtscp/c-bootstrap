#include "atl/include/string.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "Parser.h"
#include "Preprocessor.h"
#include "passes/DotGraph.h"
#include "passes/Optimiser.h"
#include "passes/SemanticAnalysis.h"

class Test_Optimiser : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_Optimiser() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = test_root + "Test_Optimiser/" + test_name + "/test.cpp";
  }
};

using namespace ACC;

TEST_F(Test_Optimiser, FunDecls) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Preprocessor preprocessor(src, {});
  ACC::Scanner scanner(preprocessor.getSource());
  Lexer lexer(scanner);
  ACC::Parser parser(lexer);
  atl::shared_ptr<ACC::Program> progAST = parser.getAST();

  SemanticAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  ASSERT_EQ(0, nameAnalysis.errorCount);

  Optimiser optimiser(progAST);
  do {
    optimiser.run();
    optimiser.printOptimisations();
  } while (optimiser.optimisationsCount > 0);
}
