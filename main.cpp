#include <iostream>
#include <string>

#include "include/ASTPasses.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/Scanner.h"

#include "include/AST.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Provide Filename" << std::endl;
    return 1;
  }

  std::string filename(argv[1]);

  ACC::Scanner scanner(filename);

  ACC::Lexer lexer(scanner);

  ACC::Parser parser(lexer);

  ACC::Program progAST = parser.parse();

  ACC::NameAnalysis nameAnalysis(progAST, &std::cout);
  nameAnalysis.run();
  if (nameAnalysis.errorCount > 0)
    std::cout << "Compiler found " << nameAnalysis.errorCount
              << " Error(s) related to naming conflicts." << std::endl;

  return 0;
}
