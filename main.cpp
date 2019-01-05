#include <iostream>
#include <string>

#include "include/AST.h"
#include "include/Parser.h"
#include "include/Preprocessor.h"
#include "include/SourceCode.h"
#include "include/passes/DotGraph.h"
#include "include/passes/NameAnalysis.h"
#include "include/passes/Optimiser.h"
#include "include/passes/TypeAnalysis.h"
#include "include/targets/GenerateMIPS.h"
#include "include/targets/GenerateX86.h"

int main(int argc, char const *argv[]) {
  bool outputGraph = false;
  bool optimise = false;
  bool outputMIPS = false;
  bool outputX86 = false;
  if (argc < 3) {
    std::cout << "Usage: acc <filename> { x86 | mips } [ --print |  --opt ]"
              << std::endl;
    return 1;
  }
  if (argc == 3) {
    std::string target(argv[2]);
    if (target == "mips")
      outputMIPS = true;
    else if (target == "x86")
      outputX86 = true;
    else {
      std::cout << "Invalid Target Arch, Must be either 'mips' or 'x86'"
                << std::endl;
      return 1;
    }
  }
  if (argc >= 4) {
    std::string flag(argv[3]);
    if (flag == "-p" || flag == "--print")
      outputGraph = true;
    if (flag == "-o" || flag == "--opt")
      optimise = true;
  }
  if (argc >= 5) {
    std::string flag(argv[4]);
    if (flag == "--print")
      outputGraph = true;
    if (flag == "--opt")
      optimise = true;
  }

  std::string abspath(argv[1]);

  ACC::Preprocessor preprocessor(abspath);
  ACC::SourceCode src = preprocessor.getSource();

  ACC::Parser parser(src);
  std::shared_ptr<ACC::Program> progAST = parser.getAST();

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

  if (optimise) {
    ACC::Optimiser optimiser(progAST);
    std::cerr << "Optimisations Made:" << std::endl;
    do {
      optimiser.run();
      optimiser.printOptimisations();
    } while (optimiser.optimisationsCount > 0);
  }

  if (outputGraph) {
    ACC::DotGraph dotGraph(progAST);
    dotGraph.print();
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
