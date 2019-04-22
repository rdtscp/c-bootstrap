enable_testing()

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include")

add_executable(runFileSystemUnitTests Test_FileSystem.cpp  main.cpp)
target_link_libraries(runFileSystemUnitTests libgtest libgmock)
add_test(FileSystemUnitTests runFileSystemUnitTests)

add_executable(runScannerUnitTests Test_Scanner.cpp  main.cpp)
target_link_libraries(runScannerUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ScannerUnitTests runScannerUnitTests)

add_executable(runASTUnitTests Test_AST.cpp  main.cpp)
target_link_libraries(runASTUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ASTUnitTests runASTUnitTests)

add_executable(runLexerUnitTests Test_Lexer.cpp main.cpp)
target_link_libraries(runLexerUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(LexerUnitTests runLexerUnitTests)

add_executable(runParserUnitTests Test_Parser.cpp  main.cpp)
target_link_libraries(runParserUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ParserUnitTests runParserUnitTests)

add_executable(runPreprocessorUnitTests Test_Preprocessor.cpp  main.cpp)
target_link_libraries(runPreprocessorUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(PreprocessorUnitTests runPreprocessorUnitTests)

add_executable(runASTAnalysisUnitTests Test_ASTAnalysis.cpp  main.cpp)
target_link_libraries(runASTAnalysisUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ASTAnalysisUnitTests runASTAnalysisUnitTests)

add_executable(runOptimiserUnitTests Test_Optimiser.cpp  main.cpp)
target_link_libraries(runOptimiserUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(OptimiserUnitTests runOptimiserUnitTests)

add_executable(runCodeGenerationUnitTests Test_CodeGeneration.cpp  main.cpp)
target_link_libraries(runCodeGenerationUnitTests libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(CodeGenerationUnitTests runCodeGenerationUnitTests)
