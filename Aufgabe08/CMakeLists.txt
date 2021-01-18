cmake_minimum_required(VERSION 3.17)
project(Aufgabe8)

set(CMAKE_CXX_STANDARD 17)
if (MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()
include_directories(Code)
include_directories(Hilfscode)
# Note: does not work if testAssets exists already as file
if(NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/testAssets)
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/testAssets)
endif()
file(COPY Code/testAssets/ DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/testAssets/ FILES_MATCHING PATTERN "*.txt")

add_executable(Aufgabe8
        Code/AtcoCommand.h
        Code/AtcoCommand.cpp
        Code/AtcoCmds.h
        Code/AtcoCmds.cpp
        Code/FileName.h
        Code/FileName.cpp
        Code/TestCode.cpp
        Code/DynArray.h
        Code/Processing.cpp
        Code/Processing.h
        Hilfscode/NATOAlphabet.h
        Hilfscode/NATOAlphabet.cpp
        Code/NumberExtractor.cpp
        Code/NumberExtractor.h
        Code/dosFarben.h
        Code/main.cpp
        Code/testMakros.h
        Code/Tests.cpp
        Code/Tests.h
        Code/Util.cpp
        Code/Util.h)