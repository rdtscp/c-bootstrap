#include <iostream>
#include <string>

#include "include/ASTPasses.h"
#include "include/CodeGeneration.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/Scanner.h"

#include "include/AST.h"

int main(int argc, char const *argv[]) {
  bool outputGraph = false;
  bool outputMIPS = false;
  if (argc < 2) {
    std::cout << "Provide Filename" << std::endl;
    return 1;
  }
  if (argc == 3) {
    std::string arg2(argv[2]);
    if (arg2 == "-p" || arg2 == "--print")
      outputGraph = true;
    if (arg2 == "--mips")
      outputMIPS = true;
  }

  std::string abspath(argv[1]);

  ACC::Scanner scanner(abspath);

  ACC::Lexer lexer(scanner);

  ACC::Parser parser(lexer);

  ACC::Program progAST = parser.parse();

  if (outputGraph) {
    ACC::DotGraph dotGraph(progAST);
    dotGraph.print();
  }

  ACC::NameAnalysis nameAnalysis(progAST);
  nameAnalysis.run();
  if (nameAnalysis.errorCount > 0) {
    nameAnalysis.printErrors();
    return 1;
  }

  ACC::TypeAnalysis typeAnalysis(progAST);
  typeAnalysis.run();
  if (typeAnalysis.errorCount > 0) {
    typeAnalysis.printErrors();
    return 1;
  }

  if (outputMIPS) {
    ACC::GenerateMIPS mipsGenerator(progAST, "mips.asm");
    mipsGenerator.run();
  }

  return 0;
}
