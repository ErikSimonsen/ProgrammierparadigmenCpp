#include "AtcoCmds.h"
#include "Processing.h"
#include "Util.h"
#include <fstream>

void AtcoCmds::ReadFromFile(std::ifstream &ifstream) {
  std::string line;
  AtcoCommand atcoCommand;

  bool firstLine = true;
  bool lastLineWasTimestamp = false;
  while (getline(ifstream, line)) {
    // timestamp line
    if (IsLineTimestamp(line)) {
      lastLineWasTimestamp = true;
      if (firstLine)
        firstLine = false;
      else // only do this if not the first utterance/skip for first command
           // block
        this->Add(atcoCommand);
      atcoCommand = AtcoCommand(); // assign to new default-constructed
                                   // class(stack object) to "reset" the state
      FileName fileName{
          line.substr(0, 23)}; // initialize filename, but remove the ':' before
      atcoCommand.SetFileName(fileName);
    } // word sequence line
    else if (IsLineWordSequence(line) || lastLineWasTimestamp) {
      atcoCommand.SetWordSequence(Trim(line));
      atcoCommand.DetermineCallSign(Trim(line));
      lastLineWasTimestamp = false;
      // command sequence line
    } else if (IsLineCommandSequence(line))
      atcoCommand.AddCommand(Trim(line));
  }
  // add last AtcoCommand too
  this->Add(atcoCommand);
}

int AtcoCmds::GetCountedUtterances() const {
  return atcoCommands.GetCntElements();
}

int AtcoCmds::GetUtterancesLimit() const { return atcoCommands.GetLimit(); }

void AtcoCmds::Add(AtcoCommand &atcoCommand) { atcoCommands.Add(atcoCommand); }

AtcoCommand AtcoCmds::Get(const int &i) { return atcoCommands.Get(i); }

void AtcoCmds::Set(const AtcoCommand &atcoCommand, const int &pos) {
  atcoCommands.Set(atcoCommand, pos);
}
