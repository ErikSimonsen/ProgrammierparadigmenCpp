#include <string>
#include <iostream>
#include <fstream>
#include "Tests.h"
#include "Util.h"
#include "AtcoCommand.h"

bool testTrim() {
    std::string a = "  aabb ccdd ";
    std::string b = "aaf   ";
    std::string c = "a ";
    std::string d = " b";
    std::string e = " ";
    if (Trim(a) != "aabb ccdd")
        return false;
    if (Trim(b) != "aaf")
        return false;
    if (Trim(c) != "a")
        return false;
    if (Trim(d) != "b")
        return false;
    if (!Trim(e).empty())
        return false;
    return true;
}

bool testSplitIntoDateTime() {
    AtcoCommand ac1;
    AtcoCommand ac2;
    ac1.filename = "2019-02-15__11-32-02-00";
    ac2.filename = "2222-02-15__11-32-22-22";

    SplitFilenameIntoDateTime(ac1);
    SplitFilenameIntoDateTime(ac2);

    if (ac1.dateTime.year != 2019 || ac1.dateTime.month != 2 || ac1.dateTime.day != 15 || ac1.dateTime.hour != 11 ||
        ac1.dateTime.minute != 32 ||
        ac1.dateTime.seconds != 2 || ac1.dateTime.milliseconds != 0) {
        return false;
    }
    if (ac2.dateTime.year != 2222 || ac2.dateTime.month != 2 || ac2.dateTime.day != 15 || ac2.dateTime.hour != 11 ||
        ac2.dateTime.minute != 32 ||
        ac2.dateTime.seconds != 22 || ac2.dateTime.milliseconds != 22) {
        return false;
    }
    return true;
}

bool testFillAllowedCommands() {
    AllowedCmdSet allowedCmdSet;
    std::ifstream inTypes("expectedTypes.txt");
    if (!inTypes) {
        std::cerr << "Die angegebene Datei für erlaubten Commands konnte nicht gefunden werden.";
        return -1;
    }
    FillAllowedCommands(allowedCmdSet, inTypes);
    const bool is_in11 = allowedCmdSet.find("DESCEND") != allowedCmdSet.end();
    const bool is_in12 = allowedCmdSet.find("CALL_YOU_BACK") != allowedCmdSet.end();
    const bool is_in13 = allowedCmdSet.find("DIRECT_TO") != allowedCmdSet.end();
    const bool is_in14 = allowedCmdSet.find("CLEARED ILS") != allowedCmdSet.end();
    const bool is_in15 = allowedCmdSet.find("CLEARED") != allowedCmdSet.end();
    if (!is_in11 || !is_in12 || !is_in13 || !is_in14 || !is_in15)
        return false;

    return true;
}

bool testFillAllowedCommands1() {
    AllowedCmdSet allowedCmdSet;
    std::ifstream inTypes("expectedTypes2.txt");
    if (!inTypes) {
        std::cerr << "Die angegebene Datei für erlaubten Commands konnte nicht gefunden werden.";
        return -1;
    }
    FillAllowedCommands(allowedCmdSet, inTypes);
    const bool is_in1 = allowedCmdSet.find("CLEARED ILS") != allowedCmdSet.end();
    const bool is_in2 = allowedCmdSet.find("REDUCE") != allowedCmdSet.end();
    const bool is_in3 = allowedCmdSet.find("CLIMB") != allowedCmdSet.end();
    const bool is_in4 = allowedCmdSet.find("INFORMATION QNH") != allowedCmdSet.end();
    const bool is_in5 = allowedCmdSet.find("INFORMATION ATIS") != allowedCmdSet.end();
    const bool is_in6 = allowedCmdSet.find("TEST1") != allowedCmdSet.end();
    const bool is_in7 = allowedCmdSet.find("TEST2") != allowedCmdSet.end();
    const bool is_in8 = allowedCmdSet.find("TEST3") != allowedCmdSet.end();
    if (!is_in1 || !is_in2 || !is_in3 || !is_in4 || !is_in5 || !is_in6 || !is_in7 || !is_in8)
        return false;
    return true;
}

//weak test
bool testFillAtcoCommands() {
    AtcoCommand atcoCommands[MaxUtterances];
    std::ifstream inFile("WordSeqPlusCmds.txt");
    FillAtcoCommands(atcoCommands, inFile);
    for (int i = 0; i < 6; ++i) {
        if (atcoCommands[i].filename.empty() || atcoCommands[i].wordSequence.empty() ||
            atcoCommands[i].commands[0].empty())
            return false;
    }
    return true;
}

bool testFillAtcoCommands1() {
    AtcoCommand atcoCommands[MaxUtterances];
    std::ifstream inFile("WordSeqPlusCmds2.txt");
    FillAtcoCommands(atcoCommands, inFile);
    for (int i = 0; i < 2; ++i) {
        if (atcoCommands[i].filename.empty() || atcoCommands[i].wordSequence.empty() ||
            atcoCommands[i].commands[0].empty() || atcoCommands[i].commands[1].empty())
            return false;
    }
    return true;
}

bool testCountWordOccurences() {
    AtcoCommand atcoCommands[MaxUtterances];
    WordCntMap wordSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds.txt");
    FillAtcoCommands(atcoCommands, inFile);
    CountWordOccurences(atcoCommands, wordSeqCntMap);
    const bool c1 = wordSeqCntMap["lufthansa"] == 4;
    const bool c2 = wordSeqCntMap["zero"] == 5;
    const bool c3 = wordSeqCntMap["radar"] == 7;
    const bool c4 = wordSeqCntMap["one"] == 8;
    const bool c5 = wordSeqCntMap["contact"] == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;
    return true;
}

bool testCountWordOccurences1() {
    AtcoCommand atcoCommands[MaxUtterances];
    WordCntMap wordSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds1.txt");
    FillAtcoCommands(atcoCommands, inFile);
    CountWordOccurences(atcoCommands, wordSeqCntMap);
    const bool c1 = wordSeqCntMap["two"] == 3;
    const bool c2 = wordSeqCntMap["charlie"] == 2;
    const bool c3 = wordSeqCntMap["speedbird"] == 1;
    if (!c1 || !c2 || !c3)
        return false;
    return true;
}

bool testCountCmdOccurences() {
    AtcoCommand atcoCommands[MaxUtterances];
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds.txt"), allowedCmds("expectedTypes.txt");
    FillAtcoCommands(atcoCommands, inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCommands, cmdSeqCntMap, allowedCmdSet);

    const bool c1 = cmdSeqCntMap["CLEARED"] == 3;
    const bool c5 = cmdSeqCntMap["CLEARED ILS"] == 1;
    const bool c2 = cmdSeqCntMap["DESCEND"] == 1;
    const bool c3 = cmdSeqCntMap["DIRECT_TO"] == 2;
    const bool c4 = cmdSeqCntMap.size() == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;
    return true;
}

bool testCountCmdOccurences1() {
    AtcoCommand atcoCommands[MaxUtterances];
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds2.txt"), allowedCmds("expectedTypes2.txt");
    FillAtcoCommands(atcoCommands, inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCommands, cmdSeqCntMap, allowedCmdSet);

    const bool c1 = cmdSeqCntMap["CLEARED ILS"] == 1;
    const bool c2 = cmdSeqCntMap["INFORMATION QNH"] == 1;
    const bool c3 = cmdSeqCntMap["INFORMATION ATIS"] == 1;
    const bool c4 = cmdSeqCntMap["CLIMB"] == 2;
    const bool c5 = cmdSeqCntMap.size() == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;

    return true;
}

bool testOrderWordOccurences() {
    AtcoCommand atcoCommands[MaxUtterances];
    WordCntMap wordSeqCntMap;
    WordCntOrderedSet wordCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds.txt");
    FillAtcoCommands(atcoCommands, inFile);
    CountWordOccurences(atcoCommands, wordSeqCntMap);
    OrderCountedWords(wordSeqCntMap, wordCntOrderedSet);
    // Iterator to the beginning of set
    auto iter = wordCntOrderedSet.begin();
    // It will move forward the passed iterator by passed value
    std::pair<std::string, int> fp1, fp2, fp3, fp4, fp5, fp6, fp7, fp8, fp9, fp10;
    fp1 = *iter;
    std::advance(iter, 1);
    fp2 = *iter;
    std::advance(iter, 1);
    fp3 = *iter;
    std::advance(iter, 1);
    fp4 = *iter;
    std::advance(iter, 1);
    fp5 = *iter;
    std::advance(iter, 1);
    fp6 = *iter;
    std::advance(iter, 1);
    fp7 = *iter;
    std::advance(iter, 1);
    fp8 = *iter;
    std::advance(iter, 1);
    fp9 = *iter;
    std::advance(iter, 1);
    fp10 = *iter;
    if ((fp1.first != "one" && fp1.second != 8) || (fp2.first != "radar" && fp2.second != 7) ||
        (fp3.first != "six" && fp3.second != 5)
        || (fp4.first != "two" && fp4.second != 5) || (fp5.first != "zero" && fp5.second != 5) ||
       ( fp6.first != "alfa" && fp6.second != 4)
        || (fp7.first != " contact" && fp7.second != 4 )|| (fp8.first != "flight" && fp8.second != 4) ||
       ( fp9.first != "level" && fp9.second != 4) || (fp10.first != "lusthansa" && fp10.second != 4))
        return false;
    return true;
}

bool testOrderWordOccurences1() {
    AtcoCommand atcoCommands[MaxUtterances];
    WordCntMap wordSeqCntMap;
    WordCntOrderedSet wordCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds1.txt");
    FillAtcoCommands(atcoCommands, inFile);
    CountWordOccurences(atcoCommands, wordSeqCntMap);
    OrderCountedWords(wordSeqCntMap, wordCntOrderedSet);
    // Iterator to the beginning of set
    auto iter = wordCntOrderedSet.begin();
    // It will move forward the passed iterator by passed value
    std::pair<std::string, int> p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
    p1 = *iter;
    std::advance(iter, 1);
    p2 = *iter;
    std::advance(iter, 1);
    p3 = *iter;
    std::advance(iter, 1);
    p4 = *iter;
    std::advance(iter, 1);
    p5 = *iter;
    std::advance(iter, 1);
    p6 = *iter;
    std::advance(iter, 1);
    p7 = *iter;
    std::advance(iter, 1);
    p8 = *iter;
    std::advance(iter, 1);
    p9 = *iter;
    std::advance(iter, 1);
    p10 = *iter;
    if ((p1.first != "two" && p1.second != 3) || (p2.first != "charlie" && p2.second != 2) ||
        (p3.first != "climb" && p3.second != 1)
        || (p4.first != "five" && p4.second != 1)|| (p5.first != "flight" && p5.second != 1) ||
        (p6.first != "good" && p6.second != 1)
        || (p7.first != "level" && p7.second != 7) || (p8.first != "morning" && p8.second != 1) ||
        (p9.first != "speedbird" && p9.second != 1) || (p10.first != "zero" && p10.second != 1))
        return false;
    return true;
}

bool testOrderCmdOccurences() {
    AtcoCommand atcoCommands[MaxUtterances];
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    CmdCntOrderedSet cmdCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds.txt"), allowedCmds("expectedTypes.txt");
    FillAtcoCommands(atcoCommands, inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCommands, cmdSeqCntMap, allowedCmdSet);
    OrderCountedCmds(cmdSeqCntMap, cmdCntOrderedSet);

    auto iter = cmdCntOrderedSet.begin();
    // It will move forward the passed iterator by passed value
    std::pair<std::string, int> p1, p2, p3, p4;
    p1 = *iter;
    std::advance(iter, 1);
    p2 = *iter;
    std::advance(iter, 1);
    p3 = *iter;
    std::advance(iter, 1);
    p4 = *iter;
    if((p1.first != "CLEARED" && p1.second != 2) || (p2.first != "DIRECT_TO" && p2.second != 2)
    || (p3.first != "CLEARED ILS" && p3.second != 1) || (p4.first != "DESCEND" && p3.second != 1))
        return false;
    return true;
}

bool testOrderCmdOccurences1() {
    AtcoCommand atcoCommands[MaxUtterances];
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    CmdCntOrderedSet cmdCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds2.txt"), allowedCmds("expectedTypes2.txt");
    FillAtcoCommands(atcoCommands, inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCommands, cmdSeqCntMap, allowedCmdSet);
    OrderCountedCmds(cmdSeqCntMap, cmdCntOrderedSet);
    auto iter = cmdCntOrderedSet.begin();
    // It will move forward the passed iterator by passed value
    std::pair<std::string, int> p1, p2, p3, p4;
    p1 = *iter;
    std::advance(iter, 1);
    p2 = *iter;
    std::advance(iter, 1);
    p3 = *iter;
    std::advance(iter, 1);
    p4 = *iter;
    if((p1.first != "CLIMB" && p1.second != 2) || (p2.first != "CLEARED ILS" && p2.second != 1) || (p3.first != "INFORMATION ATIS" && p3.second != 1)
    || (p4.first != "INFORMATION QNH" && p4.second != 1))
        return false;
    return true;
}
