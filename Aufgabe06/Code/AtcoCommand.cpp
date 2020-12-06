#include <cstring>
#include <iostream>
#include "AtcoCommand.h"
#include "Util.h"

AtcoCommand::AtcoCommand(const FileName &filename, const std::string &wordSequence)
{
    this->filename = filename; //flache Kopie
    this->wordSequence = wordSequence;
}

AtcoCommand::AtcoCommand(const AtcoCommand &copy)
{
    this->filename = copy.filename;
    this->wordSequence = copy.wordSequence;
    this->maxCommands = copy.maxCommands;
    this->cmdCnt = copy.cmdCnt;
    //no need to delete the old pointer, because it is an nullptr per default
    this->commands = new std::string[this->maxCommands];
    for (int i = 0; i < copy.maxCommands; i++)
    {
        this->commands[i] = copy.commands[i];
    }
}

AtcoCommand &AtcoCommand::operator=(const AtcoCommand &copy)
{
    if (this != &copy)
    {
        this->FreeCommands();

        this->filename = copy.filename;
        this->wordSequence = copy.wordSequence;

        this->commands = new std::string[copy.maxCommands];
        this->maxCommands = copy.maxCommands;
        this->cmdCnt = copy.cmdCnt;

        for (int i = 0; i < this->cmdCnt; i++)
        {
            this->commands[i] = copy.commands[i];
        }
    }
    return *this;
}
AtcoCommand::~AtcoCommand()
{
    this->FreeCommands();
}

std::string AtcoCommand::GetCommand(const int &i) const
{
    this->CheckAgainstCmdCnt(i);
    return this->commands[i];
}
void AtcoCommand::SetCommand(const std::string &text, const int i)
{
    this->CheckAgainstCmdCnt(i);
    this->commands[i] = text;
}
const std::string *AtcoCommand::GetCommandsPtr() const
{
    return this->commands;
}
int AtcoCommand::GetCmdCnt() const
{
    return this->cmdCnt;
}
void AtcoCommand::AddCommand(const std::string &command)
{
    if (this->commands == nullptr)
    {
        this->InitCommands();
    }

    if (this->cmdCnt == this->maxCommands)
    {
        this->Resize();
    }
    this->commands[this->cmdCnt] = command;
    this->cmdCnt += 1;
}

void AtcoCommand::CheckAgainstCmdCnt(const int &i) const
{
    if (i >= this->cmdCnt)
        throw std::out_of_range("invalid index");
}

void AtcoCommand::Resize()
{
    int newSize = this->maxCommands + 5;
    std::string *resizedArr = new std::string[newSize];

    for (int i = 0; i < this->cmdCnt; i++)
    {
        resizedArr[i] = this->commands[i];
    }

    this->FreeCommands();
    this->maxCommands = newSize;
    this->commands = resizedArr;
}

void AtcoCommand::InitCommands()
{
    this->commands = new std::string[this->maxCommands];
}

void AtcoCommand::FreeCommands()
{
    delete[] this->commands;
    this->commands = nullptr;
}

FileName AtcoCommand::GetFileName() const
{
    return this->filename;
}

std::string AtcoCommand::GetWordSequence() const
{
    return this->wordSequence;
}

void AtcoCommand::SetWordSequence(std::string const &wordSeq)
{
    this->wordSequence = wordSeq;
}
void AtcoCommand::SetFileName(FileName const &fileName)
{
    this->filename = fileName;
}