cmake_minimum_required(VERSION 3.17)
project(Aufgabe6)

set(CMAKE_CXX_STANDARD 17)
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

include_directories(Code)
configure_file(Code/expectedTypes.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/expectedTypes2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/BigWordSeqPlusCmdsFile.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/bigWordsFileExpectedTypes.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(Aufgabe6
        Code/AtcoCommand.cpp
        Code/AtcoCommand.h
        Code/FileName.cpp
        Code/FileName.h
        Code/Processing.cpp
        Code/Processing.h
        Code/dosFarben.h
        Code/main.cpp
        Code/testMakros.h
        Code/Tests.cpp
        Code/Tests.h
        Code/Util.cpp
        Code/Util.h Code/AtcoCmds.cpp Code/AtcoCmds.h)
