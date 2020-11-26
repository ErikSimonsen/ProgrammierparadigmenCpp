#ifndef AUFGABE6_ATCOCMDS_H
#define AUFGABE6_ATCOCMDS_H

#include "AtcoCommand.h"

class AtcoCmds
{
public:
    AtcoCmds(int maxUtterances = 0);
    AtcoCmds(const AtcoCmds &copy);
    AtcoCmds &operator=(const AtcoCmds &copy);
    ~AtcoCmds();

    void Add(AtcoCommand &atcoCommand);

    void ReadFromFile(std::ifstream &ifstream);
    AtcoCommand &Get(const int &i) const; //call check against utt cnt () here, return pointer?
    AtcoCommand *GetAtcoCommands() const;
    int GetMaxUtterances() const;
    int GetCountedUtterances() const;

private:
    int maxUtterances;
    int utterancesCnt = 0;
    AtcoCommand *atcoCommands;
    void Free();
    void Resize();
    void CheckAgainstUttCnt(const int &i) const;
};
#endif //AUFGABE6_ATCOCMDS_H