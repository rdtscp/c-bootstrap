cmake_minimum_required(VERSION 3.6.2)
set (CMAKE_CXX_STANDARD 14)

project(cpp_base)

IF(CMAKE_BUILD_TYPE MATCHES Release)
  message(" --- Building in Release Mode ---")
  add_library(Scanner SHARED src/Scanner.cpp)
  add_library(Preprocessor SHARED src/Preprocessor.cpp)
  add_library(Lexer SHARED src/Lexer.cpp)
  add_library(Parser SHARED src/Parser.cpp)
  add_library(DotGraph SHARED src/passes/DotGraph.cpp)
  add_library(NameAnalysis SHARED src/passes/NameAnalysis.cpp)
  add_library(TypeAnalysis SHARED src/passes/TypeAnalysis.cpp)

  add_library(GenerateMIPS SHARED src/targets/GenerateMIPS.cpp)


  target_link_libraries(Lexer Scanner)
  target_link_libraries(Preprocessor Scanner)
  target_link_libraries(Lexer Preprocessor)
  target_link_libraries(Parser Lexer)

  install(
    TARGETS GenerateMIPS TypeAnalysis NameAnalysis DotGraph Parser Lexer Preprocessor Scanner
    DESTINATION lib
  )
  install(
    FILES include/*.h
    DESTINATION include
  )

  add_executable(cpp_base main.cpp)
  target_link_libraries(cpp_base GenerateMIPS TypeAnalysis NameAnalysis DotGraph Parser Lexer Preprocessor Scanner)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
  message(" --- Building in Debug Mode ---")

  # Don't Install Google Test if its already installed.
  IF(NOT GTEST_INSTALLED MATCHES TRUE)
  
    find_package(Threads REQUIRED)
    
    # Enable ExternalProject CMake module
    include(ExternalProject)
    
    # Download and install GoogleTest
    ExternalProject_Add(
      gtest
      URL https://github.com/google/googletest/archive/release-1.8.1.zip
      PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gtest
      # Disable install step
      INSTALL_COMMAND ""
    )
    
    # Get GTest source and binary directories from CMake project
    ExternalProject_Get_Property(gtest source_dir binary_dir)

    # Create a libgtest target to be used as a dependency by test programs
    add_library(libgtest IMPORTED STATIC GLOBAL)
    add_dependencies(libgtest gtest)
    
    # Set libgtest properties
    set_target_properties(libgtest PROPERTIES
      "IMPORTED_LOCATION" "${binary_dir}/googlemock/gtest/libgtest.a"
      "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
    
    # Create a libgmock target to be used as a dependency by test programs
    add_library(libgmock IMPORTED STATIC GLOBAL)
    add_dependencies(libgmock gtest)
    
    # Set libgmock properties
    set_target_properties(libgmock PROPERTIES
      "IMPORTED_LOCATION" "${binary_dir}/googlemock/libgmock.a"
      "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
    )
  ENDIF(NOT GTEST_INSTALLED MATCHES TRUE)

  add_library(Scanner SHARED src/Scanner.cpp)
  add_library(Preprocessor SHARED src/Preprocessor.cpp)
  add_library(Lexer SHARED src/Lexer.cpp)
  add_library(Parser SHARED src/Parser.cpp)
  add_library(DotGraph SHARED src/passes/DotGraph.cpp)
  add_library(NameAnalysis SHARED src/passes/NameAnalysis.cpp)
  add_library(TypeAnalysis SHARED src/passes/TypeAnalysis.cpp)

  add_library(GenerateMIPS SHARED src/targets/GenerateMIPS.cpp)  

  target_link_libraries(Lexer Scanner)
  target_link_libraries(Preprocessor Scanner)
  target_link_libraries(Lexer Preprocessor)
  target_link_libraries(Parser Lexer)

  install(
    TARGETS GenerateMIPS TypeAnalysis NameAnalysis DotGraph Parser Lexer Preprocessor Scanner
    DESTINATION lib
  )
  install(
    FILES include/*.h
    DESTINATION include
  )

  enable_testing()
  add_subdirectory(test)
  add_executable(cpp_base main.cpp)
  target_link_libraries(cpp_base GenerateMIPS TypeAnalysis NameAnalysis DotGraph Parser Lexer Preprocessor Scanner)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)