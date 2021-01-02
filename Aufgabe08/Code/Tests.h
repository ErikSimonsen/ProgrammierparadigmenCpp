#ifndef TESTS_HEADER
#define TESTS_HEADER

bool testTrim();
bool testFileNameSplit1();
bool testFileNameSplit2();

bool testDynArrayAdd();
bool testDynArraySet();
bool testDynArrayResize();
bool testDynArrayCopyConstructor();
bool testDynArrayAssignmentOperator();

bool testAtcoCmdsReadFromFile();
bool testAtcoCmdsMoreThanSixCommands();

bool testFillAllowedCommands();
bool testFillAllowedCommands1();

bool testCountWordOccurences();
bool testCountWordOccurences1();
bool testCountCmdOccurences();
bool testCountCmdOccurences1();

bool testOrderWordOccurences();
bool testOrderWordOccurences1();
bool testOrderCmdOccurences();
bool testOrderCmdOccurences1();

bool testExtractNumbers();
bool testExtractNumberThousand();
bool testExtractDecimalNumbers();

bool testAtcoCommandDetermineCallsign();
bool testReadUtteranceCheckCallsign();
bool testReadUtteranceCheckCallsign2();

bool testReadUtteranceExtractNumbers();
bool testReadUtteranceExtractNumbers2();
#endif // TESTS_HEADER