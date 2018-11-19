#include <iostream>
#include <string>

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

  ACC::Program prog = parser.parse();

  return 0;
}
