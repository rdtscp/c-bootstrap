#include "atl/include/string.h"

#include "gtest/gtest.h"
#include "TestPath.h"

#include "Scanner.h"
#include "SourceToken.h"

using namespace ACC;

const atl::string test_prefix = test_root + "Test_Scanner/";

TEST(ScannerTest, API) {
  const atl::string filepath = test_prefix + "API/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(ScannerTest, FetchingPastEOF) {
  const atl::string filepath = test_prefix + "FetchingPastEOF/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(ScannerTest, PeekingPastEOF) {
  const atl::string filepath = test_prefix + "PeekingPastEOF/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
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

TEST(ScannerTest, ScanningPreprocessorDirectives) {
  const atl::string filepath = test_prefix + "ScanningPreprocessorDirectives/test.cpp";
  const atl::shared_ptr<SourceFileHandler> src(new SourceFileHandler(filepath));
  ACC::Scanner scanner(src);

  while (scanner.next() != '\0') {
  }
}

// The fixture for testing class Project1. From google test primer.
class Test_Scanner : public ::testing::Test {
protected:
  Test_Scanner() {
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