#ifndef TESTS_HEADER
#define TESTS_HEADER

bool testTrim();
bool testFileNameSplit1();
bool testFileNameSplit2();

bool testAtcoCmdAdd();
bool testAtcoCmdGet();
bool testAtcoCmdResize();
bool testAtcoCmdReset();
bool testAtcoCopyConstructor();
bool testAtcoAssignmentOperator();

bool testAtcoCmdsAdd();
bool testAtcoCmdsGet();
bool testAtcoCmdsResize();
bool testAtcoCmdsCopyConstructor();
bool testAtcoCmdsAssignmentOperator();
bool testAtcoCmdsReadFromFile();
bool testAtcoCmdsMoreThanSixCommands();

bool testFillAllowedCommands();
bool testFillAllowedCommands1();
bool testFillAtcoCommands();
bool testFillAtcoCommands1();

bool testCountWordOccurences();
bool testCountWordOccurences1();
bool testCountCmdOccurences();
bool testCountCmdOccurences1();

bool testOrderWordOccurences();
bool testOrderWordOccurences1();
bool testOrderCmdOccurences();
bool testOrderCmdOccurences1();

#endif //TESTS_HEADER
