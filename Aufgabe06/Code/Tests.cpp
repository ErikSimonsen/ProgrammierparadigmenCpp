#include <string>
#include <iostream>
#include <fstream>
#include "Tests.h"
#include "Util.h"
#include "AtcoCmds.h"
#include "Processing.h"
bool testTrim()
{
    std::string a = "  aabb ccdd ";
    std::string b = "aaf   ";
    std::string c = "a ";
    std::string d = " b";
    std::string e = " ";
    if (Trim(a) != "aabb ccdd" || Trim(b) != "aaf" || Trim(c) != "a" || Trim(d) != "b" || !Trim(e).empty())
    {
        return false;
    }
    return true;
}
bool testFileNameSplit1()
{
    FileName fileName1{"2016-07-12__08-33-49-01"};

    if (fileName1.GetDay() != 12 || fileName1.GetMonth() != 7 || fileName1.GetYear() != 2016 ||
        fileName1.GetHour() != 8 || fileName1.GetMinute() != 33 || fileName1.GetSeconds() != 49 || fileName1.GetMilliseconds() != 1)
        return false;
    return true;
}
bool testFileNameSplit2()
{
    try
    {
        FileName fileName2;
        fileName2.SetName("2016-07-12__08-33-49-0");
    }
    catch (const std::invalid_argument &)
    {
        return true;
    }
    return false;
}

bool testAtcoCmdAdd()
{
    FileName fileName("2016-07-12__08-34-08-02"); //stack object
    std::string wordSequence = " ryan_air seven seven delta kilo direct to detsa";
    AtcoCommand *atcoCommand = new AtcoCommand(fileName, wordSequence);
    atcoCommand->AddCommand("RYR77DK");
    atcoCommand->AddCommand("DIRECT_TO");
    atcoCommand->AddCommand("DETSA");
    if (atcoCommand->GetCmdCnt() != 3)
        return false;
    return true;
}

bool testAtcoCmdGet()
{
    AtcoCommand *atcoCommand = new AtcoCommand();
    atcoCommand->AddCommand("RYR77DK");
    atcoCommand->AddCommand("DIRECT_TO");

    if (atcoCommand->GetCommand(0) != "RYR77DK" || atcoCommand->GetCommand(1) != "DIRECT_TO")
        return false;
    return true;
}

bool testAtcoCmdResize()
{
    AtcoCommand *atcoCommand = new AtcoCommand();
    for (int i = 0; i < 5; i++)
        atcoCommand->AddCommand("RYR77DK");
    if (atcoCommand->GetCmdCnt() != 5)
        return false;
    atcoCommand->AddCommand("Test");
    if (atcoCommand->GetCmdCnt() != 6 || atcoCommand->GetCommand(5) != "Test")
        return false;
    return true;
}
bool testAtcoCopyConstructor()
{
    FileName fileName("2016-07-12__08-34-08-02");
    AtcoCommand atcoCommand(fileName, "standby");
    atcoCommand.AddCommand("RYR81BA CONTACT RADAR");
    atcoCommand.AddCommand("RYR81BA CONTACT_FREQUENCY 129.050");
    atcoCommand.AddCommand("NLY785J TRANSITION BALAD_3N");
    AtcoCommand atcoCommand2(atcoCommand);
    atcoCommand.SetCommand("test", 0);
    atcoCommand2.GetFileName().SetName("2014-07-12__08-34-08-02");
    atcoCommand2.AddCommand("NLY785J CONTACT TOWER");
    atcoCommand2.AddCommand("CONTACT TOWER");
    atcoCommand2.AddCommand("AUA917W CLIMB 160 FL");
    //check if both AtcoCommands have different amounds of commands -> there was a deep copy, no shallow, otherwise both would
    //point on the same array of commands
    if (atcoCommand2.GetCmdCnt() != 6 || atcoCommand.GetCmdCnt() != 3)
        return false;
    //primitive fields were shallow copied
    if (atcoCommand.GetCommand(0) != "test" || atcoCommand2.GetFileName().GetYear() != 2016)
        return false;
    if (atcoCommand2.GetCommand(0) != "RYR81BA CONTACT RADAR" || atcoCommand2.GetCommand(5) != "AUA917W CLIMB 160 FL")
        return false;
    return true;
}
bool testAtcoAssignmentOperator()
{
    FileName fileName{"2016-07-12__08-34-08-02"}, fileName2{"2010-07-12__08-34-08-02"};
    AtcoCommand atcoCommand{fileName, "standby"}, atcoCommand2{};
    atcoCommand2.SetFileName(fileName2);
    atcoCommand2.SetWordSequence("active");
    const std::string *oldCommandsPointer = atcoCommand.GetCommandsPtr();
    atcoCommand = atcoCommand;
    if (oldCommandsPointer != atcoCommand.GetCommandsPtr()) //test self assignment
        return false;

    atcoCommand.AddCommand("RYR81BA CONTACT RADAR");
    atcoCommand.AddCommand("RYR81BA CONTACT_FREQUENCY 129.050");
    atcoCommand.AddCommand("NLY785J TRANSITION BALAD_3N");
    atcoCommand2.AddCommand("NLY785J TRANSITION BALAD_3N");

    atcoCommand2 = atcoCommand;
    if (atcoCommand2.GetCommandsPtr() == atcoCommand.GetCommandsPtr()) //pointer und elemente wurde nicht flach, sondern tief kopiert
        return false;
    if (atcoCommand.GetFileName().GetName() != atcoCommand2.GetFileName().GetName()) //filename wurde flach kopiert, muss gleich sein
        return false;
    atcoCommand2.AddCommand("Test");
    if (atcoCommand.GetCmdCnt() == atcoCommand2.GetCmdCnt() || atcoCommand2.GetCommand(3) != "Test") //element wurde nur in atcoCommand2 eingefügt
        return false;
    return true;
}

bool testAtcoCmdsAdd()
{
    AtcoCmds atcoCmds;
    FileName fileName("2016-07-12__08-34-08-02"); //stack object
    for (int i = 0; i < 3; i++)
    {
        AtcoCommand atcoCmd{fileName, "wordSequence" + i};
        atcoCmds.Add(atcoCmd);
    }

    if (atcoCmds.GetCountedUtterances() != 3)
        return false;
    return true;
}
bool testAtcoCmdsGet()
{
    AtcoCmds atcoCmds;
    FileName fileName("2016-07-12__08-34-08-02"); //stack object
    AtcoCommand cmd{fileName, "test word sequence"};
    atcoCmds.Add(cmd);

    if (atcoCmds.Get(0).GetWordSequence() != "test word sequence")
        return false;
    return true;
}
bool testAtcoCmdsResize()
{
    AtcoCmds atcoCmds{3};
    for (int i = 0; i < 4; i++)
    {
        AtcoCommand atcoCommand;
        atcoCmds.Add(atcoCommand);
    }
    if (atcoCmds.GetMaxUtterances() != 8 || atcoCmds.GetCountedUtterances() != 4)
        return false;
    return true;
}
bool testAtcoCmdsCopyConstructor()
{
    AtcoCmds atcoCmds1;
    FileName fileName;
    for (int i = 0; i < 5; i++)
    {
        AtcoCommand atcoCmd{fileName, "command " + i};
        atcoCmds1.Add(atcoCmd);
    }

    AtcoCmds atcoCmds2{atcoCmds1};
    //both objects have to have exact the same primitive values
    if (atcoCmds2.GetMaxUtterances() != atcoCmds1.GetMaxUtterances() || atcoCmds2.GetCountedUtterances() != atcoCmds1.GetCountedUtterances())
        return false;
    AtcoCommand atcoCmd{fileName, "test"};
    atcoCmds2.Add(atcoCmd);
    //check if both pointers still point to the same array or to separate ones
    if (atcoCmds2.GetAtcoCommands() == atcoCmds1.GetAtcoCommands())
        return false;
    if (atcoCmds2.GetCountedUtterances() != 6 || atcoCmds2.GetCountedUtterances() <= atcoCmds1.GetCountedUtterances())
        return false;

    return true;
}
bool testAtcoCmdsAssignmentOperator()
{
    AtcoCmds atcoCmds1, atcoCmds2;
    AtcoCommand *oldCommandsPointer = atcoCmds1.GetAtcoCommands();
    atcoCmds1 = atcoCmds1;
    //test self assignment
    if (oldCommandsPointer != atcoCmds1.GetAtcoCommands())
        return false;
    FileName fileName;
    for (int i = 0; i < 5; i++)
    {
        AtcoCommand atcoCmd{fileName, "command " + i}, atcoCmd2{};
        atcoCmd2.SetFileName(fileName);
        atcoCmd2.SetWordSequence("Testcommand " + i);
        atcoCmds1.Add(atcoCmd);
        atcoCmds2.Add(atcoCmd2);
    }
    atcoCmds2 = atcoCmds1;

    if (atcoCmds2.GetAtcoCommands() == atcoCmds1.GetAtcoCommands()) //pointer und elemente wurde nicht flach, sondern tief kopiert
        return false;
    for (int i = 0; i < atcoCmds1.GetCountedUtterances(); i++)
    {
        AtcoCommand c1 = atcoCmds1.Get(i);
        AtcoCommand c2 = atcoCmds2.Get(i);
        AtcoCommand *ptr_c1 = &c1;
        AtcoCommand *ptr_c2 = &c2;
        if (ptr_c1 == ptr_c2) //falls objekt referenzen gleich sind -> keine tiefe Kopie erfolgt
            return false;
        if (c1.GetWordSequence() != c2.GetWordSequence()) //primitive datentypen müssen auch bei tiefer kopie übereinstimmen (da diese flach kopiert werden)
            return false;
    }
    return true;
}

bool testAtcoCmdsReadFromFile()
{
    AtcoCmds atcoCmds;
    std::ifstream inFile("WordSeqPlusCmds.txt");
    std::ifstream inFile2("WordSeqPlusCmds1.txt");
    std::ifstream inFile3("WordSeqPlusCmds2.txt");

    atcoCmds.ReadFromFile(inFile);

    if (atcoCmds.GetCountedUtterances() != 11 || atcoCmds.GetMaxUtterances() != 15)
        return false;
    atcoCmds.ReadFromFile(inFile2);
    atcoCmds.ReadFromFile(inFile3);
    if (atcoCmds.GetCountedUtterances() != 15 || atcoCmds.GetMaxUtterances() != 15)
        return false;
    for (int i = 0; i < atcoCmds.GetCountedUtterances(); i++)
    {
        AtcoCommand atcoCommand = atcoCmds.Get(i);
        if (atcoCommand.GetFileName().GetName().empty() || atcoCommand.GetWordSequence().empty() || atcoCommand.GetCommandsPtr() == nullptr)
            return false;
    }

    return true;
}
bool testAtcoCmdsMoreThanSixCommands()
{
    AtcoCmds atcoCmds;
    std::ifstream inFile("WordSeqPlusCmds1.txt");
    atcoCmds.ReadFromFile(inFile);
    AtcoCommand atcoCommand = atcoCmds.Get(0); //note: assignment constructor
    if (atcoCommand.GetCmdCnt() != 9)
        return false;
    for (int i = 1; i < 9; i++)
    {
        std::string command = Trim(atcoCommand.GetCommand(i));
        std::string cmpCommand = Trim("BAW52CV CLIMB 12" + std::to_string(i) + " FL  ");
        if (command != cmpCommand)
            return false;
    }
    return true;
}
//Processing Functions test
bool testFillAllowedCommands()
{
    AllowedCmdSet allowedCmdSet;
    std::ifstream inTypes("expectedTypes.txt");
    if (!inTypes)
    {
        std::cerr << "Die angegebene Datei für erlaubten Commands konnte nicht gefunden werden.";
        return false;
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

bool testFillAllowedCommands1()
{
    AllowedCmdSet allowedCmdSet;
    std::ifstream inTypes("expectedTypes2.txt");
    if (!inTypes)
    {
        std::cerr << "Die angegebene Datei für erlaubten Commands konnte nicht gefunden werden.";
        return false;
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

bool testCountWordOccurences()
{
    AtcoCmds atcoCmds{10};
    WordCntMap wordSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds.txt");
    atcoCmds.ReadFromFile(inFile);
    CountWordOccurences(atcoCmds, wordSeqCntMap);
    const bool c1 = wordSeqCntMap["lufthansa"] == 4;
    const bool c2 = wordSeqCntMap["zero"] == 5;
    const bool c3 = wordSeqCntMap["radar"] == 7;
    const bool c4 = wordSeqCntMap["one"] == 8;
    const bool c5 = wordSeqCntMap["contact"] == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;
    return true;
}

bool testCountWordOccurences1()
{
    AtcoCmds atcoCmds{10};
    WordCntMap wordSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds1.txt");
    atcoCmds.ReadFromFile(inFile);
    CountWordOccurences(atcoCmds, wordSeqCntMap);
    const bool c1 = wordSeqCntMap["two"] == 3;
    const bool c2 = wordSeqCntMap["charlie"] == 2;
    const bool c3 = wordSeqCntMap["speedbird"] == 1;
    if (!c1 || !c2 || !c3)
        return false;
    return true;
}

bool testCountCmdOccurences()
{
    AtcoCmds atcoCmds{50};
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds.txt"), allowedCmds("expectedTypes.txt");
    atcoCmds.ReadFromFile(inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCmds, cmdSeqCntMap, allowedCmdSet);

    const bool c1 = cmdSeqCntMap["CLEARED"] == 3;
    const bool c5 = cmdSeqCntMap["CLEARED ILS"] == 1;
    const bool c2 = cmdSeqCntMap["DESCEND"] == 1;
    const bool c3 = cmdSeqCntMap["DIRECT_TO"] == 2;
    const bool c4 = cmdSeqCntMap.size() == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;
    return true;
}

bool testCountCmdOccurences1()
{
    AtcoCmds atcoCmds{};
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    std::ifstream inFile("WordSeqPlusCmds2.txt"), allowedCmds("expectedTypes2.txt");
    atcoCmds.ReadFromFile(inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCmds, cmdSeqCntMap, allowedCmdSet);

    const bool c1 = cmdSeqCntMap["CLEARED ILS"] == 1;
    const bool c2 = cmdSeqCntMap["INFORMATION QNH"] == 1;
    const bool c3 = cmdSeqCntMap["INFORMATION ATIS"] == 1;
    const bool c4 = cmdSeqCntMap["CLIMB"] == 2;
    const bool c5 = cmdSeqCntMap.size() == 4;
    if (!c1 || !c2 || !c3 || !c4 || !c5)
        return false;

    return true;
}

bool testOrderWordOccurences()
{
    AtcoCmds atcoCmds{};
    WordCntMap wordSeqCntMap;
    WordCntOrderedSet wordCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds.txt");
    atcoCmds.ReadFromFile(inFile);
    CountWordOccurences(atcoCmds, wordSeqCntMap);
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
        (fp3.first != "six" && fp3.second != 5) || (fp4.first != "two" && fp4.second != 5) || (fp5.first != "zero" && fp5.second != 5) ||
        (fp6.first != "alfa" && fp6.second != 4) || (fp7.first != " contact" && fp7.second != 4) || (fp8.first != "flight" && fp8.second != 4) ||
        (fp9.first != "level" && fp9.second != 4) || (fp10.first != "lusthansa" && fp10.second != 4))
        return false;
    return true;
}

bool testOrderWordOccurences1()
{
    AtcoCmds atcoCmds = {};
    WordCntMap wordSeqCntMap;
    WordCntOrderedSet wordCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds1.txt");
    atcoCmds.ReadFromFile(inFile);
    CountWordOccurences(atcoCmds, wordSeqCntMap);
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
        (p3.first != "climb" && p3.second != 1) || (p4.first != "five" && p4.second != 1) || (p5.first != "flight" && p5.second != 1) ||
        (p6.first != "good" && p6.second != 1) || (p7.first != "level" && p7.second != 7) || (p8.first != "morning" && p8.second != 1) ||
        (p9.first != "speedbird" && p9.second != 1) || (p10.first != "zero" && p10.second != 1))
        return false;
    return true;
}

bool testOrderCmdOccurences()
{
    AtcoCmds atcoCmds = {};
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    CmdCntOrderedSet cmdCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds.txt"), allowedCmds("expectedTypes.txt");
    atcoCmds.ReadFromFile(inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCmds, cmdSeqCntMap, allowedCmdSet);
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
    if ((p1.first != "CLEARED" && p1.second != 2) || (p2.first != "DIRECT_TO" && p2.second != 2) || (p3.first != "CLEARED ILS" && p3.second != 1) || (p4.first != "DESCEND" && p3.second != 1))
        return false;
    return true;
}

bool testOrderCmdOccurences1()
{
    AtcoCmds atcoCmds{};
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    CmdCntOrderedSet cmdCntOrderedSet;
    std::ifstream inFile("WordSeqPlusCmds2.txt"), allowedCmds("expectedTypes2.txt");
    atcoCmds.ReadFromFile(inFile);
    FillAllowedCommands(allowedCmdSet, allowedCmds);
    CountCmdOccurences(atcoCmds, cmdSeqCntMap, allowedCmdSet);
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
    if ((p1.first != "CLIMB" && p1.second != 2) || (p2.first != "CLEARED ILS" && p2.second != 1) || (p3.first != "INFORMATION ATIS" && p3.second != 1) || (p4.first != "INFORMATION QNH" && p4.second != 1))
        return false;
    return true;
}
