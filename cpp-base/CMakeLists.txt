cmake_minimum_required(VERSION 3.9.2)
set (CMAKE_CXX_STANDARD 17)

project(cpp_base)

IF(CMAKE_BUILD_TYPE MATCHES Release)
  message(" --- Building in Release Mode ---")
  add_executable(cpp_base main.cpp src/Parser.cpp src/Scanner.cpp src/Token.cpp src/Tokeniser.cpp)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)