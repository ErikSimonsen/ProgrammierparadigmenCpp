#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Tests.h"
#include "AtcoCommand.h"
#include "dosFarben.h"
#include "testMakros.h"

int main() {
    bool result;
    PERFORM_AND_OUTPUT(testTrim);
    PERFORM_AND_OUTPUT(testSplitIntoDateTime);
    PERFORM_AND_OUTPUT(testFillAllowedCommands);
    PERFORM_AND_OUTPUT(testFillAllowedCommands1);
    PERFORM_AND_OUTPUT(testFillAtcoCommands);
    PERFORM_AND_OUTPUT(testFillAtcoCommands1);
    PERFORM_AND_OUTPUT(testCountWordOccurences);
    PERFORM_AND_OUTPUT(testCountWordOccurences1);
    PERFORM_AND_OUTPUT(testCountCmdOccurences);
    PERFORM_AND_OUTPUT(testCountCmdOccurences1);
    PERFORM_AND_OUTPUT(testOrderWordOccurences);
    PERFORM_AND_OUTPUT(testOrderWordOccurences1);
    PERFORM_AND_OUTPUT(testOrderCmdOccurences);
    PERFORM_AND_OUTPUT(testOrderCmdOccurences1);

    if (result) {
        printScreenColorOnceVal(std::cout, GREEN_SCREEN_COLOR, "Tests erfolgreich!\n");
    } else {
        printScreenColorOnceVal(std::cout, RED_SCREEN_COLOR, "Fehler in Tests aufgetreten! ***\n");
        return -1;
    }

    AtcoCommand atcoCommands[MaxUtterances];
    AllowedCmdSet allowedCmdSet;
    WordCntMap wordSeqCntMap;
    CmdCntMap cmdSeqCntMap;
    WordCntOrderedSet wordCntOrderedSet;
    CmdCntOrderedSet cmdCntOrderedSet;

    std::ifstream infile("BigWordSeqPlusCmdsFile.txt");
    std::ifstream inTypes("bigWordsFileExpectedTypes.txt");

    if (!infile) {
        std::cerr << "Die angegebene Datei für die Word-Sequenzen konnte nicht gefunden werden.";
        return -1;
    }
    if (!inTypes) {
        std::cerr << "Die angegebene Datei für die erlaubten Kommandos konnte nicht gefunden werden.";
        return -1;
    }

    FillAllowedCommands(allowedCmdSet, inTypes);
    FillAtcoCommands(atcoCommands, infile);

    CountWordOccurences(atcoCommands, wordSeqCntMap);
    CountCmdOccurences(atcoCommands, cmdSeqCntMap, allowedCmdSet);

    OrderCountedWords(wordSeqCntMap, wordCntOrderedSet);
    OrderCountedCmds(cmdSeqCntMap, cmdCntOrderedSet);

    std::cout << "ActoCommands:" << "\n";
    for (int i = 0; i <= MaxUtterances; ++i) {
        if(atcoCommands[i].filename.empty()){
            break;
        }
        std::cout << atcoCommands[i].filename << " ";
        std::cout << "(" << atcoCommands[i].dateTime.day << "-" << atcoCommands[i].dateTime.month << "-"
                  << atcoCommands[i].dateTime.year << ",";
        std::cout << " " << atcoCommands[i].dateTime.hour << ":" << atcoCommands[i].dateTime.minute << ":"
                  << atcoCommands[i].dateTime.seconds << ":" << atcoCommands[i].dateTime.milliseconds << "):\n";
        std::cout << "  " << atcoCommands[i].wordSequence << "\n";
        for (auto &command : atcoCommands[i].commands) {
            if (!command.empty())
                std::cout << "   " << command << ";  ";
        }
        std::cout << "\n";
    }


    std::cout << "Top 10 Words:" << "\n";
    int i = 0;
    for (const auto &iter: wordCntOrderedSet){
        std::cout << i << ".   " << iter.first << ":" << iter.second << "\n";
        if(i == 9 || iter.first.empty()){
            break;
        }
        i++;
    }
    std::cout << "Top 1 Commands:" << "\n";
    int highestCount = 0;
    for (const auto &iter: cmdCntOrderedSet){
        if(iter.second < highestCount){ //as soon as an element in the set has a lower count than the previous one break - this way only the top commands/command (multiple if the yhave the same count)
            break;
        }
        highestCount = iter.second;
        std::cout << iter.first << ":" << iter.second << "\n";
    }
    std::cout << "Top 10 Commands:" << "\n";
    i = 0;
    for (const auto &iter: cmdCntOrderedSet){
        std::cout << i << ".   " << iter.first << ":" << iter.second << "\n";
        if(i == 9 || iter.first.empty()){
            break;
        }
        i++;
    }
    return 0;
}