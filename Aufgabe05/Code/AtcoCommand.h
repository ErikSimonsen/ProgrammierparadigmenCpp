#ifndef AUFGABE5_ATCOCOMMAND_H
#define AUFGABE5_ATCOCOMMAND_H

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

enum {MaxCommands = 6};

struct AtcoCommand {
    DateTime dateTime{};
    std::string filename;
    std::string wordSequence;
    std::string commands[MaxCommands];
};
void Add(AtcoCommand *&atcoCommands, int &atcoOccurencesCnt, int &atcoOccurencesMax, const AtcoCommand &atcoCommand);
void Resize(AtcoCommand *&atcoCommands, int &atcoOccurencesMax);
void FillAtcoCommands(AtcoCommand *& atcoCommands, int &atcoOccurencesCnt, int &atcoOccurencesMax, std::ifstream &infile);

void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile);

void CountWordOccurences(const AtcoCommand* atcoCommands, const int &atcoOccurencesCnt, WordCntMap &wordCntMap);
void CountCmdOccurences(const AtcoCommand* atcoCommands, const int &atcoOccurencesCnt,
                        CmdCntMap &cmdCntMap, const AllowedCmdSet &allowedCmdSet);

void OrderCountedWords(const WordCntMap &wordCntMap, WordCntOrderedSet &wordCntOrderedSet);
void OrderCountedCmds(const CmdCntMap &cmdCntMap, CmdCntOrderedSet &cmdCntOrdered);
void SplitFilenameIntoDateTime(AtcoCommand &atcoCommand);

bool IsLineWordSequence(std::string &line);
bool IsLineCommandSequence(std::string &line);
bool IsLineTimestamp(std::string &line);
#endif //AUFGABE5_ATCOCOMMAND_H
