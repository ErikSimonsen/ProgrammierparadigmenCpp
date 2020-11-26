#include <sstream>
#include <fstream>
#include "AtcoCommand.h"
#include "Util.h"

void FillAtcoCommands(AtcoCommand atcoCommands[], std::ifstream &infile) {
    std::string line;
    int index = -1; //
    int commandsIndex = 0;
    while (getline(infile, line)) {
        //timestamp line
        if (IsLineTimestamp(line)) {
            index++;
            atcoCommands[index].filename = line;
            SplitFilenameIntoDateTime(atcoCommands[index]);
            commandsIndex = 0;
        }//word sequence line
        else if (IsLineWordSequence(line)) {
            atcoCommands[index].wordSequence = line;
        }//command sequence line
        else if (IsLineCommandSequence(line)) {
            atcoCommands[index].commands[commandsIndex] = line;
            commandsIndex++;
        }
    }
}

void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile) {
    std::string line;
    //read each line and then parse the line with a stringstream with the comma delimiter into words
    while (getline(infile, line)) {
        allowedCmdSet.insert(Trim(line));
    }
}

void SplitFilenameIntoDateTime(AtcoCommand &atcoCommand) {
    if (!atcoCommand.filename.empty()) {
        atcoCommand.dateTime.year = std::stoi(atcoCommand.filename.substr(0, 4));
        atcoCommand.dateTime.month = std::stoi(atcoCommand.filename.substr(5, 2));
        atcoCommand.dateTime.day = std::stoi(atcoCommand.filename.substr(8, 2));
        atcoCommand.dateTime.hour = std::stoi(atcoCommand.filename.substr(12, 2));
        atcoCommand.dateTime.minute = std::stoi(atcoCommand.filename.substr(15, 2));
        atcoCommand.dateTime.seconds = std::stoi(atcoCommand.filename.substr(18, 2));
        atcoCommand.dateTime.milliseconds = std::stoi(atcoCommand.filename.substr(21, 2));
    }
}

void CountWordOccurences(const AtcoCommand atcoCommands[], WordCntMap &wordCntMap) {
    std::string word;
    for (int i = 0; i < MaxUtterances; ++i) {
        if (atcoCommands[i].wordSequence.empty())
            break;

        std::stringstream strStream(atcoCommands[i].wordSequence);
        while (strStream >> word) {
            wordCntMap[word]++;
        }
    }
}

void OrderCountedWords(const WordCntMap &wordCntMap, WordCntOrderedSet &wordCntOrderedSet) {
    for (const auto &iter: wordCntMap) {
        std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
        wordCntOrderedSet.insert(pair);
    }
}

void CountCmdOccurences(const AtcoCommand atcoCommands[], CmdCntMap &cmdCntMap,
                        const AllowedCmdSet &allowedCmdSet) {
    for (int i = 0; i < MaxUtterances; ++i) {
        if (atcoCommands[i].filename.empty())
            break;
        for (const auto &command : atcoCommands[i].commands) {
            if (command.empty()) {
                break;
            }
            std::stringstream strStream(command);
            std::string callSign, type, extendedType;
            strStream >> callSign >> type >> extendedType;
            //check if type is allowed, otherwise check if the command is the result of the typ and the extended type (e.g. CLEARED ILS)

            if (!extendedType.empty() && !isdigit(extendedType[0])) {
                std::string fullType = type + " " + extendedType; //do not use type.append() here! because then the string object for type also gets changed which results in the .find(type) in the if further down to return false
                if (allowedCmdSet.find(fullType) != allowedCmdSet.end()) {
                    cmdCntMap[fullType]++;
                }
            }
            if (allowedCmdSet.find(type) != allowedCmdSet.end()) {
                cmdCntMap[type]++;
            }
        }
    }
}

void OrderCountedCmds(const CmdCntMap &cmdCntMap, CmdCntOrderedSet &cmdCntOrderedSet) {
    for (const auto &iter: cmdCntMap) {
        std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
        cmdCntOrderedSet.insert(pair);
    }
}

bool IsLineWordSequence(std::string &line) {
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && islower(line[3]);
}

bool IsLineCommandSequence(std::string &line) {
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && isupper(line[3]);
}

bool IsLineTimestamp(std::string &line) {
    return isdigit(line[0]) && line[0] == '2';
}