cmake_minimum_required(VERSION 3.6.2)
set (CMAKE_CXX_STANDARD 14)

project(acc)

include(ExternalProject)
ExternalProject_Add(atl
  GIT_REPOSITORY    https://github.com/rdtscp/atl
  GIT_TAG           master
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/atl
  # Disable install step
  INSTALL_COMMAND ""
)
include_directories("${CMAKE_CURRENT_BINARY_DIR}/atl/src/")

add_library(AST SHARED
  src/ast/ArrayAccess.cpp
  src/ast/ArrayType.cpp
  src/ast/Assign.cpp
  src/ast/BaseType.cpp
  src/ast/BinOp.cpp
  src/ast/Block.cpp
  src/ast/CharLiteral.cpp
  src/ast/DoWhile.cpp
  src/ast/EnumTypeDecl.cpp
  src/ast/FieldAccess.cpp
  src/ast/FunCall.cpp
  src/ast/FunDecl.cpp
  src/ast/FunDef.cpp
  src/ast/If.cpp
  src/ast/IntLiteral.cpp
  src/ast/Namespace.cpp
  src/ast/ParenthExpr.cpp
  src/ast/PointerType.cpp
  src/ast/Program.cpp
  src/ast/Return.cpp
  src/ast/SizeOf.cpp
  src/ast/StringLiteral.cpp
  src/ast/StructType.cpp
  src/ast/StructTypeDecl.cpp
  src/ast/TypeCast.cpp
  src/ast/TypeDefDecl.cpp
  src/ast/ValueAt.cpp
  src/ast/VarDecl.cpp
  src/ast/VarExpr.cpp
  src/ast/While.cpp
)
add_library(DotGraph SHARED src/passes/DotGraph.cpp)
add_library(GenerateMIPS SHARED src/targets/GenerateMIPS.cpp)
add_library(GenerateX86 SHARED src/targets/GenerateX86.cpp)
add_library(Lexer SHARED src/Lexer.cpp)
add_library(MIPS SHARED src/targets/MIPS.cpp)
add_library(NameAnalysis SHARED src/passes/NameAnalysis.cpp)
add_library(Optimiser SHARED src/passes/Optimiser.cpp)
add_library(Parser SHARED src/Parser.cpp)
add_library(TypeAnalysis SHARED src/passes/TypeAnalysis.cpp)
add_library(Scanner SHARED src/Scanner.cpp)
add_library(X86 SHARED src/targets/X86.cpp)

add_dependencies(Scanner atl)

target_link_libraries(AST MIPS X86)
target_link_libraries(DotGraph AST)
target_link_libraries(GenerateMIPS AST MIPS)
target_link_libraries(GenerateX86  AST X86)
target_link_libraries(Lexer Scanner)
target_link_libraries(NameAnalysis AST)
target_link_libraries(Optimiser AST)
target_link_libraries(Parser AST Lexer)
target_link_libraries(TypeAnalysis AST)

install(
  TARGETS AST
          DotGraph
          GenerateMIPS
          GenerateX86
          Lexer
          MIPS
          NameAnalysis
          Optimiser
          Parser
          Scanner
          TypeAnalysis
          X86
  DESTINATION lib
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include
  DESTINATION include
)

IF(CMAKE_BUILD_TYPE MATCHES Debug)
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
  enable_testing()
  add_subdirectory(test)
ENDIF(CMAKE_BUILD_TYPE MATCHES Debug)

add_executable(acc main.cpp)
add_dependencies(acc atl)
target_link_libraries(acc AST DotGraph GenerateMIPS GenerateX86 NameAnalysis Optimiser Parser TypeAnalysis)