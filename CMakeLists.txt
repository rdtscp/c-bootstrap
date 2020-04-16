cmake_minimum_required(VERSION 3.6.2)
set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_FLAGS "-Wall -Wno-unused-function -Werror")

IF (CMAKE_BUILD_TYPE MATCHES Debug)
  IF (NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
    message("Enabling LeakSanitizer")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=leak")
    set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -g -fno-omit-frame-pointer -fsanitize=leak")
  ENDIF (NOT CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
ENDIF (CMAKE_BUILD_TYPE MATCHES Debug)

# Code Coverage Configuration
option(CODE_COVERAGE "Enable coverage reporting" OFF)
IF (CODE_COVERAGE)
  message("Enabling Code Coverage")
  add_library(coverage_config INTERFACE)

  set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g --coverage")
  set (CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -O0 -g --coverage")
  if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
    target_link_options(coverage_config INTERFACE --coverage)
  else()
    target_link_libraries(coverage_config INTERFACE --coverage)
  endif()
ENDIF (CODE_COVERAGE)

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
include_directories("${CMAKE_SOURCE_DIR}/include/")

add_library(AST SHARED
  src/ast/AddressOf.cpp
  src/ast/Allocation.cpp
  src/ast/ArrayType.cpp
  src/ast/Assign.cpp
  src/ast/BaseType.cpp
  src/ast/BinOp.cpp
  src/ast/Block.cpp
  src/ast/BoolLiteral.cpp
  src/ast/CharLiteral.cpp
  src/ast/ClassType.cpp
  src/ast/ClassTypeDecl.cpp
  src/ast/ClassTypeDef.cpp
  src/ast/ConstructorCall.cpp
  src/ast/ConstructorDecl.cpp
  src/ast/ConstructorDef.cpp
  src/ast/Deletion.cpp
  src/ast/DestructorDecl.cpp
  src/ast/DestructorDef.cpp
  src/ast/DoWhile.cpp
  src/ast/EnumClassTypeDecl.cpp
  src/ast/For.cpp
  src/ast/FunCall.cpp
  src/ast/FunDecl.cpp
  src/ast/FunDef.cpp
  src/ast/FunSignature.cpp
  src/ast/Identifier.cpp
  src/ast/If.cpp
  src/ast/IntLiteral.cpp
  src/ast/MemberAccess.cpp
  src/ast/MemberCall.cpp
  src/ast/Namespace.cpp
  src/ast/Nullptr.cpp
  src/ast/ParenthExpr.cpp
  src/ast/PointerType.cpp
  src/ast/PrefixOp.cpp
  src/ast/Program.cpp
  src/ast/ReferenceType.cpp
  src/ast/Return.cpp
  src/ast/SizeOf.cpp
  src/ast/StaticCast.cpp
  src/ast/StringLiteral.cpp
  src/ast/SubscriptOp.cpp
  src/ast/TertiaryExpr.cpp
  src/ast/Throw.cpp
  src/ast/TypeDefDecl.cpp
  src/ast/ValueAt.cpp
  src/ast/VarDef.cpp
  src/ast/VarDecl.cpp
  src/ast/VarExpr.cpp
  src/ast/While.cpp
)
add_library(DotGraph SHARED src/passes/DotGraph.cpp)
add_library(GenerateX86 SHARED src/targets/GenerateX86.cpp)
add_library(Lexer SHARED src/Lexer.cpp)
add_library(Optimiser SHARED src/passes/Optimiser.cpp)
add_library(Parser SHARED src/Parser.cpp)
add_library(Preprocessor SHARED src/Preprocessor.cpp)
add_library(SemanticAnalysis SHARED src/passes/SemanticAnalysis.cpp)
add_library(SourceOutput SHARED src/passes/SourceOutput.cpp)
add_library(Scanner SHARED src/Scanner.cpp)
add_library(X86 SHARED src/targets/X86.cpp)

add_dependencies(AST atl)
add_dependencies(GenerateX86 atl)
add_dependencies(Lexer atl)
add_dependencies(Optimiser atl)
add_dependencies(Parser atl)
add_dependencies(Preprocessor atl)
add_dependencies(Scanner atl)
add_dependencies(SemanticAnalysis atl)
add_dependencies(X86 atl)

target_link_libraries(AST X86)
target_link_libraries(DotGraph AST)
target_link_libraries(GenerateX86  AST X86)
target_link_libraries(Lexer Scanner)
target_link_libraries(Optimiser AST)
target_link_libraries(Parser AST Lexer)
target_link_libraries(Preprocessor Scanner)
target_link_libraries(SemanticAnalysis AST)
target_link_libraries(SourceOutput AST)

install(
  TARGETS AST
          DotGraph
          GenerateX86
          Lexer
          Optimiser
          Parser
          Preprocessor
          Scanner
          SemanticAnalysis
          SourceOutput
          X86
  DESTINATION lib
)

install(
  DIRECTORY ${CMAKE_SOURCE_DIR}/include
  DESTINATION include
)

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

add_executable(acc main.cpp)
add_dependencies(acc atl)
target_link_libraries(acc AST DotGraph GenerateX86 Optimiser Parser Preprocessor Scanner SemanticAnalysis SourceOutput)