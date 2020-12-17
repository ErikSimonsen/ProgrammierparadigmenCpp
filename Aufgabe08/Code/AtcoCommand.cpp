#include <cstring>
#include <iostream>
#include "AtcoCommand.h"
#include "Util.h"

AtcoCommand::AtcoCommand(const FileName &filename, const std::string &wordSequence)
{
    this->filename = filename; //flache Kopie
    this->wordSequence = wordSequence;
}
void AtcoCommand::AddCommand(const std::string &command) {
    commands.Add(command);
}

FileName AtcoCommand::GetFileName() const
{
    return filename;
}

std::string AtcoCommand::GetWordSequence() const
{
    return wordSequence;
}

void AtcoCommand::SetWordSequence(std::string const &wordSeq)
{
    wordSequence = wordSeq;
}
void AtcoCommand::SetFileName(FileName const &fileName)
{
    filename = fileName;
}

std::string AtcoCommand::GetCommand(const int &i)
{
    return commands.Get(i);
}

const std::string *AtcoCommand::GetCommandsPtr() const
{
    return commands.GetArrPtr();
}
void AtcoCommand::SetCommand(const std::string &text, const int &i)
{
    commands.Set(text, i);
}

int AtcoCommand::GetCmdCnt() const
{
    return commands.GetCntElements();
}