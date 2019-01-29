#include <iostream>
#include <string>

#include "atl/include/string.h"

#include "include/AST.h"
#include "include/Parser.h"
#include "include/passes/DotGraph.h"
#include "include/passes/NameAnalysis.h"
#include "include/passes/Optimiser.h"
#include "include/passes/TypeAnalysis.h"
#include "include/targets/GenerateMIPS.h"
#include "include/targets/GenerateX86.h"

int main(int argc, char const *argv[]) {
  if (argc < 4) {
    std::cout << "Usage: acc <input> <output> { x86 | mips } [ --print "
                 "|  --opt ]"
              << std::endl;
    return 1;
  }
  const atl::string inFilename(argv[1]);
  const std::string outFilename(argv[2]);
  const std::string target(argv[3]);

  bool outputMIPS = false;
  bool outputX86 = false;
  if (target == "mips")
    outputMIPS = true;
  else if (target == "x86")
    outputX86 = true;
  else {
    std::cout << "Invalid Target Arch, Must be either 'mips' or 'x86'"
              << std::endl;
    return 1;
  }

  bool outputGraph = false;
  bool optimise = false;
  if (argc > 3) {
    for (int i = 4; i < argc; i++) {
      const std::string flag(argv[i]);
      if (flag == "-p" || flag == "--print")
        outputGraph = true;
      if (flag == "-o" || flag == "--opt")
        optimise = true;
    }
  }

  ACC::SourceHandler src(ACC::SourceHandler::Type::FILEPATH, inFilename);
  ACC::Scanner scanner(src);
  ACC::Lexer lexer(scanner);
  ACC::Parser parser(lexer);
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
    ACC::GenerateMIPS mipsGenerator(progAST, outFilename);
    mipsGenerator.run();
  }

  if (outputX86) {
    ACC::GenerateX86 x86Generator(progAST, outFilename);
    x86Generator.run();
  }

  return 0;
}
