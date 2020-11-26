#ifndef AUFGABE6_PROCESSING_H
#define AUFGABE6_PROCESSING_H

#include <map>
#include <set>
#include "AtcoCmds.h"
struct CompareCount
{
    bool operator()(const std::pair<std::string, int> &a, const std::pair<std::string, int> &b) const
    {
        //return the bigger number (the count for the word), if they are equal however sort by the name
        if (a.second == b.second)
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

void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile);

void CountWordOccurences(const AtcoCmds &atcoCommands, WordCntMap &wordCntMap);

void CountCmdOccurences(const AtcoCmds &atcoCommands, CmdCntMap &cmdCntMap, const AllowedCmdSet &allowedCmdSet);

void OrderCountedWords(const WordCntMap &wordCntMap, WordCntOrderedSet &wordCntOrderedSet);

void OrderCountedCmds(const CmdCntMap &cmdCntMap, CmdCntOrderedSet &cmdCntOrdered);

bool IsLineWordSequence(std::string &line);

bool IsLineCommandSequence(std::string &line);

bool IsLineTimestamp(std::string &line);
#endif //AUFGABE6_PROCESSING_H