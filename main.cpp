#include "atl/include/string.h"

#include "include/AST.h"
#include "include/Parser.h"
#include "include/Preprocessor.h"
#include "include/passes/DotGraph.h"
#include "include/passes/Optimiser.h"
#include "include/passes/SemanticAnalysis.h"
#include "include/passes/SourceOutput.h"
#include "include/targets/GenerateX86.h"

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("Usage: acc <input> <output> [ --graph |  --opt | --preprocess, "
           "--source ]\n");
    return 1;
  }
  const atl::string inFilename(argv[1]);
  const atl::string outFilename(argv[2]);

  bool outputGraph = false;
  bool optimise = false;
  bool preprocess = false;
  bool source = false;
  if (argc > 3) {
    for (int i = 4; i < argc; ++i) {
      const atl::string flag(argv[i]);
      if (flag == "--graph")
        outputGraph = true;
      if (flag == "--opt")
        optimise = true;
      if (flag == "--preprocess")
        preprocess = true;
      if (flag == "--source")
        source = true;
    }
  }

  if (preprocess) {
    ACC::SourceHandler src(ACC::SourceHandler::Type::FILEPATH, inFilename);
    ACC::Preprocessor preprocessor(src, {"/Users/alexanderwilson/Documents/"
                                         "GitHub/c-bootstrap/build/atl/src/",
                                         "/Users/alexanderwilson/Documents/"
                                         "GitHub/c-bootstrap/include/"});
    ACC::SourceHandler pp_src = preprocessor.getSource();
    printf("%s", pp_src.value.c_str());

    return 0;
  } else {
    ACC::SourceHandler src(ACC::SourceHandler::Type::FILEPATH, inFilename);
    ACC::Preprocessor preprocessor(src, {"/Users/alexanderwilson/Documents/"
                                         "GitHub/c-bootstrap/build/atl/src/",
                                         "/Users/alexanderwilson/Documents/"
                                         "GitHub/c-bootstrap/include/"});
    ACC::Scanner scanner(preprocessor.getSource());
    ACC::Lexer lexer(scanner);
    ACC::Parser parser(lexer);
    atl::shared_ptr<ACC::Program> progAST = parser.getAST();

    ACC::SemanticAnalysis semanticAnalysis(progAST);
    semanticAnalysis.run();
    if (semanticAnalysis.errorCount > 0) {
      semanticAnalysis.printErrors();
      return 1;
    }

    if (optimise) {
      ACC::Optimiser optimiser(progAST);
      printf("Optimisations Made:\n");
      do {
        optimiser.run();
        optimiser.printOptimisations();
      } while (optimiser.optimisationsCount > 0);
    }

    if (outputGraph) {
      ACC::DotGraph dotGraph(progAST);
      dotGraph.print();
    }

    if (source) {
      ACC::SourceOutput sourceOutput(progAST);
      sourceOutput.print();
    }

    ACC::GenerateX86 x86Generator(progAST, outFilename);
    x86Generator.run();

    return 0;
  }
}
