#include <iostream>
#include <string>

#include "include/Parser.h"
#include "include/Scanner.h"
#include "include/Tokeniser.h"

int main(int argc, char const *argv[]) {
  if (argc < 2) {
    std::cout << "Provide Filename" << std::endl;
    return 1;
  }

  std::string filename(argv[1]);

  ACC::Scanner scanner(filename);

  ACC::Tokeniser tokeniser(scanner);

  ACC::Parser parser(tokeniser);

  ACC::Program prog = parser.parse();
  
  /* code */
  return 0;
}
