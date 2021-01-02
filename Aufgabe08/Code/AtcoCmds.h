#ifndef AUFGABE8_ATCOCMDS_H
#define AUFGABE8_ATCOCMDS_H
#include "AtcoCommand.h"
#include "DynArray.h"

class AtcoCmds {
public:
  /**
   * No copy-constructor and assignment operator are needed because they are
   * implemented in DynArray.cpp (which is the only Class that allocates Heap
   * Memory). In the default generated assignment operator or copy constructor
   * of this class a flat copy of each member var is made which trough the
   * assignment of this->atcoCommands = copy.atcoCommands calls the assignment
   * operator of the DynArray, which then correctly deletes the old memory and
   * allocates the new. For the case that we have an DynArray<AtcoCommand> whose
   * AtcoCommand-Objects have an DynArray<std::string> themselves this default
   * assignment operator will call the default assignment operator of the
   * AtcoCommand class which then, trough the assignment of this->commands =
   * copy.commands calls the assignment operator its own member
   * DynArray<std::string>, which then gets resolved (first the assignment
   * opreator of DynArray<std::string> is processed and afterwards the programm
   * bubbles up and resolves the assignment operator of DynArray<AtcoCommand>.
   */
  void Add(AtcoCommand &atcoCommand);

  void ReadFromFile(std::ifstream &ifstream);
  AtcoCommand Get(const int &pos);
  void Set(const AtcoCommand &atcoCommand, const int &pos);
  int GetUtterancesLimit() const;
  int GetCountedUtterances() const;

private:
  DynArray<AtcoCommand> atcoCommands;
};
#endif // AUFGABE8_ATCOCMDS_H