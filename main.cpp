#include "atl/include/string.h"

#include "include/AST.h"
#include "include/Parser.h"
#include "include/Preprocessor.h"
#include "include/passes/DotGraph.h"
#include "include/passes/Optimiser.h"
#include "include/passes/SemanticAnalysis.h"
#include "include/passes/SourceOutput.h"
#include "include/targets/GenerateX64.h"

int main(int argc, char const *argv[]) {
  if (argc < 3) {
    printf("Usage: acc <input> <output> [ --graph |  --opt | "
           "--preprocess ]\n");
    return 1;
  }
  const atl::string inFilename(argv[1]);
  const atl::string outFilename(argv[2]);

  bool outputGraph = false;
  bool optimise = false;
  bool preprocess = false;
  if (argc > 2) {
    for (int i = 3; i < argc; ++i) {
      const atl::string flag(argv[i]);
      if (flag == "--graph")
        outputGraph = true;
      if (flag == "--opt")
        optimise = true;
      if (flag == "--preprocess")
        preprocess = true;
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

    const atl::string temp_s_filename = "temp.s";
    const atl::string temp_o_filename = "temp.o";
    if (outputGraph) {
      ACC::DotGraph dotGraph(progAST, outFilename);
      dotGraph.print();
    } else {
      ACC::GenerateX64 x64Generator(progAST, temp_s_filename);
      x64Generator.run();
    }

    const atl::string nasm_cmd = "nasm -f macho64 " + temp_s_filename;
    const int nasm_status = system(nasm_cmd.c_str());
    if (nasm_status != 0) {
      return 1;
    }
    const atl::string ld_cmd = "ld -no_pie -macosx_version_min 10.15 -lSystem -o " + outFilename + " " + temp_o_filename;
    const int ld_status = system(ld_cmd.c_str());
    if (ld_status != 0) {
      return 1;
    }
    return 0;
  }
}
