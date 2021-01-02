#include "AtcoCommand.h"
#include "../Hilfscode/NATOAlphabet.h"
#include "Util.h"
#include <sstream>
#include <vector>
AtcoCommand::AtcoCommand(const FileName &filename,
                         const std::string &wordSequence) {
  this->filename = filename; // flache Kopie
  this->wordSequence = wordSequence;
}
void AtcoCommand::AddCommand(const std::string &command) {
  commands.Add(command);
}

FileName AtcoCommand::GetFileName() const { return filename; }

std::string AtcoCommand::GetWordSequence() const { return wordSequence; }

std::string AtcoCommand::GetCallSign() const { return callSign; }

void AtcoCommand::SetWordSequence(std::string const &wordSeq) {
  wordSequence = wordSeq;
}
void AtcoCommand::DetermineCallSign(std::string const &wordSeq) {
  std::istringstream isstream(wordSeq);
  std::vector<std::string> words;
  std::string _callSign;
  std::string word;
  while (isstream >> word) {
    words.push_back(word);
  }
  // for each word it is checked if the word itself (or combined with up to 3
  // next words, inner loop) are in the Nato Alphabet
  for (auto iter = words.begin(); iter != words.end(); iter++) {
    std::string currentWord = "";
    // need to check up to 3 additional words, for example "united states of
    // america"""
    size_t callSignBefore = _callSign.size();
    for (auto innerIt = iter; innerIt != words.end(); innerIt++) {
      // inner iterator is at the end of the
      // vector -> return, because that means no
      // words in the nato alphabet are left
      if (innerIt == words.end() || std::distance(iter, innerIt) > 3) {
        break;
      }
      // concatenate the word 2nd/3rd/4th word to the first word
      currentWord += " " + (*innerIt);
      currentWord = Trim(currentWord);
      if (currentWord == "correction") {
        _callSign = "";
        callSignBefore = 0;
        break; // inner loop
      }
      if (NATOAlphabet::IsInNatoAlphabet(currentWord)) {
        std::string natoRepresentation =
            NATOAlphabet::GetNatoRepresentation(currentWord);
        // if the nato representation is a multiplier then get the next word and
        // if its in the nato alphabet -> concatenate the nato representation of
        // the next word to the call sign (2 times when multiplier word was
        // double, 3 times when triple etc.);
        if (NATOAlphabet::IsMultiplier(currentWord)) {
          auto nextIter = std::next(innerIt);
          std::string nextWord = *nextIter;
          if (NATOAlphabet::IsInNatoAlphabet(nextWord)) {
            std::string nextNatoRepresentation =
                NATOAlphabet::GetNatoRepresentation(nextWord);
            // concatenate the representation of the next word multiple times
            // (multiplier) to the callsign
            for (int i = 0; i < std::stoi(natoRepresentation); ++i) {
              _callSign += nextNatoRepresentation;
            }
            iter = nextIter; // skip the next word from being processed again
          }
          break;
        } else {
          _callSign += NATOAlphabet::GetNatoRepresentation(currentWord);
          // skip the processing of the next words in the outer
          // loop, which belong to the current word (e.g. if the current word is
          // "united" and the next 3 words are "states of america" then these 3
          // next words dont have to be processed again
          iter = innerIt;
          break;
        }
      }
    }
    // if the call sign has the same size after the processing of the current
    // word like before the processing and already contained a callSign(or a
    // part of it) return, because that means that the just processed word does
    // not belong to the call sign
    if (callSignBefore > 0 && callSignBefore == _callSign.size()) {
      this->callSign = _callSign;
      return;
    }
  }
}
void AtcoCommand::SetFileName(FileName const &fileName) { filename = fileName; }

std::string AtcoCommand::GetCommand(const int &i) { return commands.Get(i); }

const std::string *AtcoCommand::GetCommandsPtr() const {
  return commands.GetArrPtr();
}
void AtcoCommand::SetCommand(const std::string &text, const int &i) {
  commands.Set(text, i);
}

int AtcoCommand::GetCmdCnt() const { return commands.GetCntElements(); }