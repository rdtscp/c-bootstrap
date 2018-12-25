#include <iostream>
#include <string>

#include "include/AST.h"
#include "include/Lexer.h"
#include "include/Parser.h"
#include "include/Scanner.h"
#include "include/passes/DotGraph.h"
#include "include/passes/NameAnalysis.h"
#include "include/passes/TypeAnalysis.h"
#include "include/targets/GenerateMIPS.h"
#include "include/targets/GenerateX86.h"

int main(int argc, char const *argv[]) {
  bool outputGraph = false;
  bool outputMIPS = false;
  bool outputX86 = false;
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
    if (arg2 == "--x86")
      outputX86 = true;
  }

  std::string abspath(argv[1]);

  ACC::Scanner scanner(abspath);

  ACC::Lexer lexer(scanner);

  ACC::Parser parser(lexer);

  std::shared_ptr<ACC::Program> progAST = parser.parse();

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

  if (outputX86) {
    ACC::GenerateX86 x86Generator(progAST, "x86.s");
    x86Generator.run();
  }

  return 0;
}
