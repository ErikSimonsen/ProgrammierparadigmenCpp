#include "Processing.h"
#include "AtcoCmds.h"
#include "AtcoCommand.h"
#include "NumberExtractor.h"
#include "Util.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <setjmp.h>
#include <sstream>

// General functions, which dont need to be separated in a class, because they
// are called only once anyways (in main)
void FillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile) {
  std::string line;
  // read each line and then parse the line with a stringstream with the comma
  // delimiter into words
  while (getline(infile, line)) {
    allowedCmdSet.insert(Trim(line));
  }
}

void CountWordOccurences(AtcoCmds &atcoCmds, WordCntMap &wordCntMap) {
  std::string word;
  for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i) {
    std::stringstream strStream(atcoCmds.Get(i).GetWordSequence());
    while (strStream >> word) {
      wordCntMap[word]++;
    }
  }
}

void OrderCountedWords(const WordCntMap &wordCntMap,
                       WordCntOrderedSet &wordCntOrderedSet) {
  for (const auto &iter : wordCntMap) {
    std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
    wordCntOrderedSet.insert(pair);
  }
}

void CountCmdOccurences(AtcoCmds &atcoCmds, CmdCntMap &cmdCntMap,
                        const AllowedCmdSet &allowedCmdSet) {
  for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i) {
    AtcoCommand currentAtcoCommand = atcoCmds.Get(i);
    for (int j = 0; j < currentAtcoCommand.GetCmdCnt(); j++) {
      std::string command = currentAtcoCommand.GetCommand(j);

      std::stringstream strStream(command); // parse string by whitespace
      std::string callSign, type, extendedType;
      strStream >> callSign >> type >> extendedType;
      // check if extended type exists and then if the type concatenated with
      // extended type is an allowed command
      if (!extendedType.empty() && !isdigit(extendedType[0])) {
        std::string fullType =
            type + " " +
            extendedType; // do not use type.append() here! because then the
                          // string object for type also gets changed which
                          // results in the .find(type) in the if further down
                          // to return false
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

void OrderCountedCmds(const CmdCntMap &cmdCntMap,
                      CmdCntOrderedSet &cmdCntOrderedSet) {
  for (const auto &iter : cmdCntMap) {
    std::pair<std::string, int> pair = std::make_pair(iter.first, iter.second);
    cmdCntOrderedSet.insert(pair);
  }
}

bool ReadUtteranceCheckCallsign(std::string fileName, bool printOut,
                                std::vector<std::string> expectedValues) {
  AtcoCmds atcoCmds{};
  std::ifstream inFile(fileName);
  bool result = true;
  if (!inFile) {
    std::cerr << "Die angegebene Word-Sequenz Datei: " + fileName +
                     " konnte im Build-Verzeichnis nicht gefunden werden."
              << "\n";
    return false;
  }
  atcoCmds.ReadFromFile(inFile);
  int expectedLength = static_cast<int>(expectedValues.size());

  for (int i = 0; i < expectedLength; ++i) {
    AtcoCommand atcoCommand = atcoCmds.Get(i);
    if (printOut) {
      std::cout << atcoCommand.GetWordSequence() << "\n";
      std::cout << "      determined:" + atcoCommand.GetCallSign()
                << "  expected:" << expectedValues.at(i) << "\n";
    }
    if (atcoCommand.GetCallSign() != expectedValues.at(i)) {
      result = false;
    }
  }
  return result;
}
bool ReadUtteranceExtractNumbers(
    std::string fileName, std::vector<std::vector<double>> expectedValues,
    bool printOut) {
  AtcoCmds atcoCmds{};
  std::ifstream inFile(fileName);
  bool result = true;
  if (!inFile) {
    std::cerr << "Die angegebene Word-Sequenz Datei: " + fileName +
                     " konnte im Build-Verzeichnis nicht gefunden werden."
              << "\n";
    return false;
  }
  atcoCmds.ReadFromFile(inFile);
  auto expectedIter = expectedValues.begin();
  for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i) {
    AtcoCommand atcoCommand = atcoCmds.Get(i);
    std::string test = atcoCommand.GetWordSequence();
    NumberExtractor numEx{atcoCommand.GetWordSequence()};
    numEx.PerformFullExtraction();
    if (printOut) {
      std::cout << atcoCommand.GetWordSequence() << "\n";
    }
    std::vector<double> expectedUtteranceValues = expectedValues.at(i);

    for (int j = 0; j < numEx.GetExtractedNumbersCnt(); ++j) {
      int expectedValuesSize = static_cast<int>(expectedUtteranceValues.size());
      if (expectedValuesSize <= j) { // there are more numbers extracted
                                     // than expected -> false
        result = false;
        continue;
      }
      if (printOut)
        std::cout << "      extracted: ";
      if (numEx.IsNumberDouble(j)) {
        if (numEx.GetNumberAsDouble(j) != expectedUtteranceValues.at(j)) {
          result = false;
        }
        if (printOut)
          std::cout << "  " << numEx.GetNumberAsDouble(j);
      } else {
        if (numEx.GetNumberAsInt(j) != expectedUtteranceValues.at(j)) {
          result = false;
        }
        if (printOut)
          std::cout << "  " << numEx.GetNumberAsInt(j);
      }
      if (printOut) {
        if (std::fmod(expectedUtteranceValues.at(j), 1) >
            0) { // hat dezimalstellen, da *expectedIter % 1 > 0
          std::cout << "    expected:   " +
                           std::to_string(expectedUtteranceValues.at(j))
                    << "\n";
        } else { // keine dezimalstellen -> muss nicht als double dargestellt
                 // werden
          std::cout << "    expected:   " +
                           std::to_string((int)expectedUtteranceValues.at(j))
                    << "\n";
        }
      }
    }
  }
  return result;
}
bool IsLineWordSequence(std::string &line) {
  return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) &&
         islower(line[3]);
}

bool IsLineCommandSequence(std::string &line) {
  return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) &&
         isupper(line[3]);
}

bool IsLineTimestamp(std::string &line) {
  return isdigit(line[0]) && line[0] == '2';
}
