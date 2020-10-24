#include "atl/include/string.h"

#include "TestPath.h"
#include "gtest/gtest.h"

#include "Scanner.h"
#include "SourceToken.h"

class Test_Scanner : public ::testing::Test {
protected:
  atl::string t_source_file;

  Test_Scanner() {
    const atl::string test_name =
        ::testing::UnitTest::GetInstance()->current_test_info()->name();

    t_source_file = test_root + "Test_Scanner/" + test_name + "/test.cpp";
  }
};

using namespace ACC;

TEST_F(Test_Scanner, API) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Scanner scanner(src);

  atl::string output;
  ASSERT_EQ(scanner.getPosition().column, 1);
  ASSERT_EQ(scanner.getPosition().line, 1);

  output += scanner.next();
  ASSERT_EQ(output, "1");
  ASSERT_EQ(scanner.peek(), '2');

  output += scanner.next();
  ASSERT_EQ(output, "12");
  ASSERT_EQ(scanner.peek(), '3');

  output += scanner.next();
  ASSERT_EQ(output, "123");
  ASSERT_EQ(scanner.peek(), '4');

  output += scanner.next();
  ASSERT_EQ(output, "1234");
  ASSERT_EQ(scanner.peek(), '5');

  output += scanner.next();
  ASSERT_EQ(output, "12345");
  ASSERT_EQ(scanner.peek(), '\n');

  output += scanner.next();
  ASSERT_EQ(output, "12345\n");
  ASSERT_EQ(scanner.peek(), '6');

  scanner.next();
  scanner.next();

  ASSERT_EQ(scanner.next(), '8');
  ASSERT_EQ(scanner.next(), '9');

  ASSERT_EQ(scanner.next(), 'a');
  ASSERT_EQ(scanner.next(), '\n');
  ASSERT_EQ(scanner.next(), '\n');
  ASSERT_EQ(scanner.next(), ' ');
  ASSERT_EQ(scanner.peek(), '\0');
}

TEST_F(Test_Scanner, FetchingPastEOF) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Scanner scanner(src);
  ASSERT_EQ(scanner.next(), 'c');
  ASSERT_EQ(scanner.next(), 'h');
  ASSERT_EQ(scanner.next(), 'a');
  ASSERT_EQ(scanner.next(), 'r');
  ASSERT_EQ(scanner.next(), 's');
  ASSERT_EQ(scanner.next(), '\0');

  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
}

TEST_F(Test_Scanner, PeekingPastEOF) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Scanner scanner(src);
  ASSERT_EQ(scanner.next(), 'c');
  ASSERT_EQ(scanner.peek(), 'h');
  ASSERT_EQ(scanner.next(), 'h');
  ASSERT_EQ(scanner.peek(), 'a');
  ASSERT_EQ(scanner.next(), 'a');
  ASSERT_EQ(scanner.peek(), 'r');
  ASSERT_EQ(scanner.next(), 'r');
  ASSERT_EQ(scanner.peek(), 's');
  ASSERT_EQ(scanner.next(), 's');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');

  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
  ASSERT_EQ(scanner.next(), '\0');
  ASSERT_EQ(scanner.peek(), '\0');
}

TEST_F(Test_Scanner, ScanningPreprocessorDirectives) {
  const atl::shared_ptr<SourceFileHandler> src(
      new SourceFileHandler(t_source_file));
  ACC::Scanner scanner(src);

  while (scanner.next() != '\0') {
  }
}
