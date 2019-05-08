enable_testing()

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include")

add_executable(runTest_AST Test_AST.cpp  main.cpp)
target_link_libraries(runTest_AST libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ASTUnitTests runTest_AST)

add_executable(runTest_ASTAnalysis Test_ASTAnalysis.cpp  main.cpp)
target_link_libraries(runTest_ASTAnalysis libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(ASTAnalysisUnitTests runTest_ASTAnalysis)

add_executable(runTest_CodeGeneration Test_CodeGeneration.cpp  main.cpp)
target_link_libraries(runTest_CodeGeneration libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(CodeGenerationUnitTests runTest_CodeGeneration)

add_executable(runTest_FileSystem Test_FileSystem.cpp  main.cpp)
target_link_libraries(runTest_FileSystem libgtest libgmock)
add_test(FileSystemUnitTests runTest_FileSystem)

add_executable(runTest_Lexer Test_Lexer.cpp main.cpp)
target_link_libraries(runTest_Lexer libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(LexerUnitTests runTest_Lexer)

add_executable(runTest_Optimiser Test_Optimiser.cpp  main.cpp)
target_link_libraries(runTest_Optimiser libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(OptimiserUnitTests runTest_Optimiser)

add_executable(runTest_Parser Test_Parser.cpp  main.cpp)
target_link_libraries(runTest_Parser libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(ParserUnitTests runTest_Parser)

add_executable(runTest_Preprocessor Test_Preprocessor.cpp  main.cpp)
target_link_libraries(runTest_Preprocessor libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser Preprocessor TypeAnalysis)
add_test(PreprocessorUnitTests runTest_Preprocessor)

add_executable(runTest_Scanner Test_Scanner.cpp  main.cpp)
target_link_libraries(runTest_Scanner libgtest libgmock AST DotGraph GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)
add_test(ScannerUnitTests runTest_Scanner)
