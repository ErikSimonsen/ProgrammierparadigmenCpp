#include <fstream>
#include "AtcoCmds.h"
#include "Processing.h"
AtcoCmds::AtcoCmds(int maxSize)
{
    this->maxUtterances = maxSize;
    this->atcoCommands = new AtcoCommand[maxSize];
}
AtcoCmds::AtcoCmds(const AtcoCmds &copy)
{
    this->maxUtterances = copy.maxUtterances;
    this->utterancesCnt = copy.utterancesCnt;

    this->atcoCommands = new AtcoCommand[copy.maxUtterances];
    for (int i = 0; i < copy.utterancesCnt; i++)
    {
        this->atcoCommands[i] = copy.atcoCommands[i];
    }
}

AtcoCmds &AtcoCmds::operator=(const AtcoCmds &copy)
{
    if (this != &copy)
    {
        this->Free();

        this->maxUtterances = copy.maxUtterances;
        this->utterancesCnt = copy.utterancesCnt;

        this->atcoCommands = new AtcoCommand[copy.maxUtterances];

        for (int i = 0; i < copy.maxUtterances; i++)
        {
            this->atcoCommands[i] = copy.atcoCommands[i];
        }
    }
    return *this;
}
AtcoCmds::~AtcoCmds()
{
    this->Free();
}

void AtcoCmds::Add(AtcoCommand &atcoCommand)
{
    if (this->utterancesCnt == this->maxUtterances)
    {
        this->Resize();
    }
    this->atcoCommands[this->utterancesCnt] = atcoCommand; //assignment operator of AtcoCommand
    this->utterancesCnt += 1;
}

AtcoCommand &AtcoCmds::Get(const int &i) const
{
    this->CheckAgainstUttCnt(i);

    return this->atcoCommands[i];
}

void AtcoCmds::Resize()
{
    size_t newSize = this->maxUtterances + 5;
    AtcoCommand *resizedArr = new AtcoCommand[newSize];

    for (int i = 0; i < this->utterancesCnt; i++)
    {
        //here the assignment constructor for the atcoCommand class is needed
        resizedArr[i] = this->atcoCommands[i];
    }

    this->Free();
    this->maxUtterances = newSize;
    this->atcoCommands = resizedArr;
}

void AtcoCmds::CheckAgainstUttCnt(const int &i) const
{
    if (i >= this->utterancesCnt)
        throw std::out_of_range("invalid index");
}

void AtcoCmds::ReadFromFile(std::ifstream &ifstream)
{
    std::string line;
    AtcoCommand atcoCommand;

    int latestIdx;
    while (getline(ifstream, line))
    {
        //timestamp line
        if (IsLineTimestamp(line))
        {
            atcoCommand = AtcoCommand{};           //assign to new default-constructed class(stack object) to "reset" the state
            FileName fileName{line.substr(0, 23)}; //initialize filename, but remove the ':' before
            this->Add(atcoCommand);
            latestIdx = this->utterancesCnt - 1;
            //remember that assigning the latest AtcoCommand to another (temporary) one would not work because of the
            //assignment constructor
            this->Get(latestIdx).SetFileName(fileName);
        } //word sequence line
        else if (IsLineWordSequence(line))
        {
            this->Get(latestIdx).SetWordSequence(line);
        } //command sequence line
        else if (IsLineCommandSequence(line))
        {
            this->Get(latestIdx).AddCommand(line);
        }
    }
}

void AtcoCmds::Free()
{
    delete[] this->atcoCommands;
    this->atcoCommands = nullptr;
}

int AtcoCmds::GetCountedUtterances() const
{
    return this->utterancesCnt;
}

int AtcoCmds::GetMaxUtterances() const
{
    return this->maxUtterances;
}

AtcoCommand *AtcoCmds::GetAtcoCommands() const
{
    return this->atcoCommands;
}