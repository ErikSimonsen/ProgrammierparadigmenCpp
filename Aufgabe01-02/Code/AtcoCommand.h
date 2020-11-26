#ifndef AUFGABE01_02_ATCOCOMMAND_H
#define AUFGABE01_02_ATCOCOMMAND_H

#include <string>
#include "DateTime.h"
#include <map>
#include <set>
struct CompareCount {
    bool operator() (const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) const {
        //return the bigger number (the count for the word), if they are equal however sort by the name
        if(a.second == b.second)
            return a.first < b.first;
        else
            return a.second > b.second;
    }
};
typedef std::set<std::string> AllowedCmdSet;
typedef std::map<std::string, int, std::less<>> WordCntMap;
typedef std::set<std::pair<std::string, int>, CompareCount> WordCntOrderedSet;
typedef std::map<std::string, int, std::less<>> CmdCntMap;
typedef std::set<std::pair<std::string, int>, CompareCount> CmdCntOrderedSet;

enum {MaxUtterances = 1000, MaxCommands = 6};

struct AtcoCommand {
    DateTime dateTime{};
    std::string filename;
    std::string wordSequence;
    std::string commands[MaxCommands];
};
void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile);
void FillAtcoCommands(AtcoCommand atcoCommands[], std::ifstream &infile);

void CountWordOccurences(const AtcoCommand atcoCommands[], WordCntMap &wordCntMap);
void OrderCountedWords(const WordCntMap &wordCntMap, WordCntOrderedSet &wordCntOrdered);
void CountCmdOccurences(const AtcoCommand atcoCommands[], CmdCntMap &cmdCntMap, const AllowedCmdSet &allowedCmdSet);
void OrderCountedCmds(const CmdCntMap &cmdCntMap, CmdCntOrderedSet &cmdCntOrdered);
void SplitFilenameIntoDateTime(AtcoCommand &atcoCommand);

bool IsLineWordSequence(std::string &line);
bool IsLineCommandSequence(std::string &line);
bool IsLineTimestamp(std::string &line);
#endif //AUFGABE01_02_ATCOCOMMAND_H
