#include <string>
#include <iostream>
#include <fstream>
#include <cmath>
#include "Tests.h"
#include "Util.h"
#include "AtcoCmds.h"
#include "Processing.h"
#include "NumberExtractor.h"
#include "DynArray.h"

const std::string TestFilesPath = "testAssets/";
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
bool testDynArrayAdd()
{
    DynArray<std::string> dynArr;
    DynArray<AtcoCommand> dynArr1;
    FileName fileName{"2016-07-12__08-33-49-01"};
    AtcoCommand atcoCommand{fileName, "test word sequence"};
    dynArr.Add("test");

    dynArr1.Add(atcoCommand);

    if (dynArr.Get(0) != "test" || dynArr1.Get(0).GetWordSequence() != "test word sequence")
        return false;
    return true;
}
bool testDynArraySet()
{
    DynArray<AtcoCommand> dynArr;
    FileName fileName{"2016-07-12__08-33-49-01"};
    AtcoCommand atcoCommand{fileName, "test word sequence"};
    AtcoCommand atcoCommand2{fileName, "test 2"};

    dynArr.Add(atcoCommand);

    dynArr.Set(atcoCommand2, 0);
    if (dynArr.Get(0).GetWordSequence() != "test 2")
        return false;
    return true;
}
bool testDynArrayResize()
{
    DynArray<std::string> dynArr{2};
    for (int i = 0; i < 3; i++)
    {
        std::string val = "string " + std::to_string(i);
        dynArr.Add(val);
    }
    if (dynArr.GetLimit() != 7 || dynArr.GetCntElements() != 3 || dynArr.Get(2) != "string 2")
        return false;
    return true;
}
bool testDynArrayCopyConstructor()
{
    DynArray<AtcoCommand> dynArr;
    FileName fileName("2016-07-12__08-34-08-02");
    for (int i = 0; i < 2; i++)
    {
        dynArr.Add(AtcoCommand{fileName, "command " + std::to_string(i)});
    }
    DynArray<AtcoCommand> dynArr2{dynArr};

    if (dynArr.GetArrPtr() == dynArr2.GetArrPtr() || dynArr.GetCntElements() != dynArr2.GetCntElements())
        return false;
    return true;
}
bool testDynArrayAssignmentOperator()
{
    DynArray<AtcoCommand> dynArr;
    DynArray<AtcoCommand> dynArr2;
    FileName fileName("2016-07-12__08-34-08-02");
    for (int i = 0; i < 2; i++)
    {
        AtcoCommand atcoCommand{fileName, "command " + i};
        atcoCommand.AddCommand("test" + i);
        atcoCommand.AddCommand("test2" + i);
        dynArr.Add(atcoCommand);
        dynArr2.Add(atcoCommand);
    }
    dynArr2 = dynArr;

    if (dynArr.GetArrPtr() == dynArr2.GetArrPtr()) //check if the pointers to the array of AtcoCommand are different
        return false;
    for (int i = 0; i < dynArr2.GetCntElements(); i++)
    {
        //check for each AtcoCommand (which holds an DynArray<std::string>) if the pointer to its DynArray<std::string> are different (deep copied)
        if (dynArr2.Get(i).GetCommandsPtr() == dynArr.Get(i).GetCommandsPtr())
            return false;
    }
    return true;
}

bool testAtcoCmdsReadFromFile()
{
    AtcoCmds atcoCmds;
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds.txt");
    std::ifstream inFile2(TestFilesPath + "WordSeqPlusCmds1.txt");
    std::ifstream inFile3(TestFilesPath + "WordSeqPlusCmds2.txt");

    atcoCmds.ReadFromFile(inFile);

    if (atcoCmds.GetCountedUtterances() != 11 || atcoCmds.GetUtterancesLimit() != 15)
        return false;
    atcoCmds.ReadFromFile(inFile2);
    atcoCmds.ReadFromFile(inFile3);
    if (atcoCmds.GetCountedUtterances() != 15 || atcoCmds.GetUtterancesLimit() != 15)
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds1.txt");
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
    std::ifstream inTypes(TestFilesPath + "expectedTypes.txt");
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
    std::ifstream inTypes(TestFilesPath + "expectedTypes2.txt");
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
    AtcoCmds atcoCmds;
    WordCntMap wordSeqCntMap;
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds.txt");
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
    AtcoCmds atcoCmds;
    WordCntMap wordSeqCntMap;
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds1.txt");
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
    AtcoCmds atcoCmds;
    AllowedCmdSet allowedCmdSet;
    CmdCntMap cmdSeqCntMap;
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds.txt"), allowedCmds(TestFilesPath + "expectedTypes.txt");
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds2.txt"), allowedCmds(TestFilesPath + "expectedTypes2.txt");
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds.txt");
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds1.txt");
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds.txt"), allowedCmds(TestFilesPath + "expectedTypes.txt");
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
    std::ifstream inFile(TestFilesPath + "WordSeqPlusCmds2.txt"), allowedCmds(TestFilesPath + "expectedTypes2.txt");
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

NumberExtractor testForIntNumbers(std::string &wordSeq, std::vector<int> &expNumbers, bool &success){
    NumberExtractor numEx(wordSeq);
    numEx.PerformFullExtraction();
    success = true;
    int expectedLength = static_cast<int>(expNumbers.size());
    for(int i = 0; i < expectedLength; ++i){
        if(!numEx.IsNumberInt(i) || numEx.GetNumberAsInt(i) != expNumbers.at(i)){
            success = false;
            break;
        }
    }
    return numEx; 
}

NumberExtractor testForDecimalNumbers(std::string &wordSeq, std::vector<double> &expNumbers, bool &success){
    NumberExtractor numEx(wordSeq);
    numEx.PerformFullExtraction();
    success = true;
    int expectedLength = static_cast<int>(expNumbers.size());

    for(int i = 0; i < expectedLength; ++i){
        if(!numEx.IsNumberDouble(i) || numEx.GetNumberAsDouble(i) != expNumbers.at(i)){
            success = false;
            break;
        }
    } 
    return numEx;
}
bool testExtractNumbers()
{
    std::string utter = "fly_niki six hundred zulu contact tower now "
                        "one two three point eight servus";

    NumberExtractor numEx(utter);
    NumberExtractor::ExtractedNumber exNum = numEx.ExtractNextFullNumber();
    if (exNum.GetExtractedNumberAsInt() != 600 || exNum.GetNumberStringSeq() != "six hundred")
        return false;
    exNum = numEx.ExtractNextFullNumber();
    if ((fabs(exNum.GetExtractedNumberAsDouble() - 123.8) > 0.0001) || (exNum.GetNumberStringSeq() != "one two three point eight"))
        return false;
    if (exNum.GetNumberOfStringsForNumber() != 5)
        return false;
    exNum = numEx.ExtractNextFullNumber();
    if ((exNum.IsExtractedNumberValid()) || (exNum.GetNumberStringSeq() != ""))
        return false;
    return true;
}
bool dumpExtractNumbers(){
    std::vector<std::string> wordSeq = {
        "dobry den sky_travel five eight juliett ruzyne radar radar contact"
        " on present heading descend four thousand feet qnh one zero two two",
        "jet_executive give two seven turn right heading three five zero vectoring for "
        " ils approach runway two four and descend five thousand feet qnh one zero one two",
        "good day vueling eight six five two praha radar radar contact "
        "descend flight level one zero zero current information is quebec and speed "
        "two seventy or less",
        "oscar kilo victor india kilo roger descend three thousand five hundred feet "
        "squawk seven thousand",
        "csa six six nine descend flight level one hundred turn left "
        " heading zero six five",
        "snow cab two hundred one descend eight thousand feet",
        "snow cab two hundred twenty one descend eight thousand six hundred feet",
        "snow cab two hundred twenty four "
        "descend eight thousand six hundred twenty one feet",
        "austrian three nine two  papa descend altitude one zero thousand "
        "qnh one zero zero three",
        "contact director one one nine dummy eight goodbye"
    };
    for(auto &val: wordSeq){
        NumberExtractor numEx{val};
        numEx.PerformFullExtraction();
        std::cout << val << "\n";
        for (int i = 0; i < numEx.GetExtractedNumbersCnt(); i++)
        {
            std::cout << "  " << numEx.GetNumberAsInt(i);
        }
        std::cout << "\n";
        
    }
    return true;
}
bool dumpExtractNumbers2(){
        std::vector<std::string> wordSeq = {
        "snow cab two hundred one descend eight thousand feet",
        "snow cab two hundred twenty one descend eight thousand six hundred feet",
        "snow cab two hundred twenty four "
        "descend eight thousand six hundred twenty one feet",
        "austrian three nine two  papa descend altitude one zero thousand "
        "qnh one zero zero three",
        "contact director one one nine dummy eight goodbye"
    };
    for(auto &val: wordSeq){
        NumberExtractor numEx{val};
        numEx.PerformFullExtraction();
        std::cout << val << "\n";
        for (int i = 0; i < numEx.GetExtractedNumbersCnt(); i++)
        {
            std::cout << "  " << numEx.GetNumberAsInt(i);
        }
        std::cout << "\n";
        
    }
    return true;
}
bool testExtractNumberThousand(){
    std::string wordSeq = "descend one zero thousand feet";
    std::vector<int> expNumbers = {10000};
    bool b_success = true;
    NumberExtractor numEx = testForIntNumbers(wordSeq, expNumbers, b_success);

    std::string wordSeq2 = "on qnh one thousand zero nine";
    std::vector<int> expNumbers2 = {1009};
    bool b_success2 = true;
    numEx = testForIntNumbers(wordSeq2, expNumbers2, b_success2);

    std::string wordSeq3 = "on qnh one thousand zero twenty bye bye";
    std::vector<int> expNumbers3 = {1020};
    bool b_success3 = true;
    numEx = testForIntNumbers(wordSeq3, expNumbers3, b_success3);

    std::string wordSeq4 = "on qnh one thousand twenty eight reduce one six zero knots";
    std::vector<int> expNumbers4 = {1028, 160};
    bool b_success4 = true;
    numEx = testForIntNumbers(wordSeq4, expNumbers4, b_success4);
    
    std::string wordSeq5 = "on qnh one thousand zero two nine";
    std::vector<int> expNumbers5 = {1029};
    bool b_success5 = true;
    numEx = testForIntNumbers(wordSeq5, expNumbers5, b_success5);

    return b_success && b_success2 && b_success3 && b_success4 && b_success5;
}

bool testExtractDecimalNumbers(){
    std::string wordSeq = "contact director one one nine decimal eight four goodbye"; 
    std::vector<double> expNumbers = {119.84};
    bool b_success = true;
    NumberExtractor numEx = testForDecimalNumbers(wordSeq, expNumbers, b_success);

    std::string wordSeq2 = "contact director one one nine point eight twenty one"; 
    std::vector<double> expNumbers2 = {119.821};
    bool b_success2 = true;
    numEx = testForDecimalNumbers(wordSeq2, expNumbers2, b_success2);

    return b_success && b_success2;
}