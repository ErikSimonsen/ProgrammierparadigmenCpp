#include <fstream>
#include <sstream>
#include "Processing.h"
#include "Util.h"
#include "AtcoCommand.h"
#include "AtcoCmds.h"
//General functions, which dont need to be separated in a class, because they are called only once anyways (in main)
void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile)
{
    std::string line;
    //read each line and then parse the line with a stringstream with the comma delimiter into words
    while (getline(infile, line))
    {
        allowedCmdSet.insert(Trim(line));
    }
}

void CountWordOccurences(AtcoCmds &atcoCmds, WordCntMap &wordCntMap)
{
    std::string word;
    for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i)
    {
        std::stringstream strStream(atcoCmds.Get(i).GetWordSequence());
        while (strStream >> word)
        {
            wordCntMap[word]++;
        }
    }
}

void OrderCountedWords(const WordCntMap &wordCntMap, WordCntOrderedSet &wordCntOrderedSet)
{
    for (const auto &iter : wordCntMap)
    {
        std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
        wordCntOrderedSet.insert(pair);
    }
}

void CountCmdOccurences(AtcoCmds &atcoCmds, CmdCntMap &cmdCntMap, const AllowedCmdSet &allowedCmdSet)
{
    for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i)
    {
        AtcoCommand currentAtcoCommand = atcoCmds.Get(i);
        for (int j = 0; j < currentAtcoCommand.GetCmdCnt(); j++)
        {
            std::string command = currentAtcoCommand.GetCommand(j);

            std::stringstream strStream(command); //parse string by whitespace
            std::string callSign, type, extendedType;
            strStream >> callSign >> type >> extendedType;
            //check if extended type exists and then if the type concatenated with extended type is an allowed command
            if (!extendedType.empty() && !isdigit(extendedType[0]))
            {
                std::string fullType = type + " " + extendedType; //do not use type.append() here! because then the string object for type also gets changed which results in the .find(type) in the if further down to return false
                if (allowedCmdSet.find(fullType) != allowedCmdSet.end())
                {
                    cmdCntMap[fullType]++;
                }
            }
            if (allowedCmdSet.find(type) != allowedCmdSet.end())
            {
                cmdCntMap[type]++;
            }
        }
    }
}

void OrderCountedCmds(const CmdCntMap &cmdCntMap, CmdCntOrderedSet &cmdCntOrderedSet)
{
    for (const auto &iter : cmdCntMap)
    {
        std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
        cmdCntOrderedSet.insert(pair);
    }
}

bool IsLineWordSequence(std::string &line)
{
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && islower(line[3]);
}

bool IsLineCommandSequence(std::string &line)
{
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && isupper(line[3]);
}

bool IsLineTimestamp(std::string &line)
{
    return isdigit(line[0]) && line[0] == '2';
}
