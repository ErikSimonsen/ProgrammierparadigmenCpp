cmake_minimum_required(VERSION 3.17)
project(Aufgabe01_02)

set(CMAKE_CXX_STANDARD 14)

include_directories(Code)
configure_file(Code/expectedTypes.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/expectedTypes2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds1.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/WordSeqPlusCmds2.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/BigWordSeqPlusCmdsFile.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)
configure_file(Code/bigWordsFileExpectedTypes.txt ${CMAKE_CURRENT_BINARY_DIR} COPYONLY)

add_executable(Aufgabe01_02
        Code/main.cpp
        Code/testMakros.h Code/Tests.cpp Code/Tests.h Code/AtcoCommand.cpp Code/AtcoCommand.h Code/DateTime.cpp Code/DateTime.h Code/Util.cpp Code/Util.h Code/dosFarben.h)
