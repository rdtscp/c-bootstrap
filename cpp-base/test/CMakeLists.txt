enable_testing()

# I couldn't make it work with INTERFACE_INCLUDE_DIRECTORIES
include_directories("${source_dir}/googletest/include")

ADD_EXECUTABLE(runLexerUnitTests Test_Lexer.cpp main.cpp)
TARGET_LINK_LIBRARIES(runLexerUnitTests
libgtest
libgmock
Parser
Scanner
Tokeniser
)
add_test(LexerUnitTests runLexerUnitTests)

ADD_EXECUTABLE(runParserUnitTests Test_Parser.cpp  main.cpp)
TARGET_LINK_LIBRARIES(runParserUnitTests
    libgtest
    libgmock
    Parser
    Scanner
    Tokeniser
)
add_test(ParserUnitTests runParserUnitTests)

ADD_EXECUTABLE(runScannerUnitTests Test_Scanner.cpp  main.cpp)
TARGET_LINK_LIBRARIES(runScannerUnitTests
    libgtest
    libgmock
    Parser
    Scanner
    Tokeniser
)
add_test(ScannerUnitTests runScannerUnitTests)
