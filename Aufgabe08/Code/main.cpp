#include "AtcoCmds.h"
#include "NumberExtractor.h"
#include "Processing.h"
#include "Tests.h"
#include "dosFarben.h"
#include "testMakros.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

bool ExecuteTests();
void ReadUtterancesAndOutputNumbers();
int main(int argc, char *argv[]) {
  /**  std::vector<std::string> expectedCallsigns = {
      "ODINK", "BAW12B", "AYY77DK", "NO_CALLSIGN"};
  std::vector<double> expectedNumbers = {4000, 1012, 4000, 10, 7000};
  ReadUtteranceCheckCallsign("testAssets/CallsignCorrection.txt", true,
                             expectedCallsigns);
  ReadUtteranceExtractNumbers("testAssets/NumberCorrection.txt",
                              expectedNumbers, true);**/
  ExecuteTests();
  if (argc > 1 && std::string(argv[1]) == "--test") {
    //  auto finish = std::chrono::system_clock::now() +
    //  std::chrono::minutes{1};
    // do
    // {
    if (ExecuteTests()) {
      printScreenColorOnceVal(std::cout, GREEN_SCREEN_COLOR,
                              "Tests erfolgreich!\n");
      return 0;
    } else {
      printScreenColorOnceVal(std::cout, RED_SCREEN_COLOR,
                              "Fehler in Tests aufgetreten! ***\n");
      return -1;
    }
    // } while (std::chrono::system_clock::now() < finish);
  } else {
    ReadUtterancesAndOutputNumbers();
  }

  return 0;
}
bool ExecuteTests() {
  bool result;
  bool finalResult = true;
  PERFORM_AND_OUTPUT(testTrim);
  // This has to be done everytime because the value of result is changed (by
  // reference) after every macro call -> even if a test fails, as long as the
  // next test succeeds the result is true -> in a calling function you cant
  // check for the variable result to determine if all tests succeeded, so
  // instead result is checked after every test
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testFileNameSplit1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testFileNameSplit2);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testDynArrayAdd);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testDynArraySet);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testDynArrayResize);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testDynArrayCopyConstructor);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testDynArrayAssignmentOperator);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testAtcoCmdsReadFromFile);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testAtcoCmdsMoreThanSixCommands);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testFillAllowedCommands);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testFillAllowedCommands1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testCountWordOccurences);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testCountWordOccurences1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testCountCmdOccurences);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testCountCmdOccurences1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testOrderWordOccurences);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testOrderWordOccurences1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testOrderCmdOccurences);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testOrderCmdOccurences1);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testExtractNumbers);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testExtractNumberThousand);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testExtractDecimalNumbers);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testAtcoCommandDetermineCallsign);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testReadUtteranceCheckCallsign);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testReadUtteranceCheckCallsign2);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testReadUtteranceExtractNumbers);
  if (result == false)
    finalResult = false;
  PERFORM_AND_OUTPUT(testReadUtteranceExtractNumbers2);
  if (result == false)
    finalResult = false;
  return finalResult;
}

void ReadUtterancesAndOutputNumbers() {

  AtcoCmds atcoCmds{};
  AllowedCmdSet allowedCmdSet;
  WordCntMap wordSeqCntMap;
  CmdCntMap cmdSeqCntMap;
  WordCntOrderedSet wordCntOrderedSet;
  CmdCntOrderedSet cmdCntOrderedSet;
  std::string assetsPath = "testAssets/";
  std::string inFilePath = assetsPath + "ChallengesMaster.txt";
  std::string inTypesPath = assetsPath + "bigWordsFileExpectedTypes.txt";
  std::ifstream infile(inFilePath);
  std::ifstream inTypes(inTypesPath);

  if (!infile) {
    std::cerr << "Die angegebene Word-Sequenz Datei: " + inFilePath +
                     " konnte im Build-Verzeichnis nicht gefunden werden."
              << "\n";
    std::cerr << "Falls das Verzeichnis " + assetsPath +
                     " nicht in ihrem Build-Verzeichnis vorhanden ist"
                     ", löschen Sie das Build-Verzeichnis und bauen Sie "
                     "nocheinmal die CMake Datei.";
    return;
  }
  if (!inTypes) {
    std::cerr << "Die angegebene Erlaubte-Kommandos Datei: " + inTypesPath +
                     " konnte im Build-Verzeichnis nicht gefunden werden."
              << "\n";
    std::cerr << "Falls das Verzeichnis " + assetsPath +
                     " nicht in ihrem Build-Verzeichnis vorhanden ist"
                     ", löschen Sie das Build-Verzeichnis und bauen Sie "
                     "nocheinmal die CMake Datei.";
    return;
  }

  FillAllowedCommands(allowedCmdSet, inTypes);
  atcoCmds.ReadFromFile(infile);
  CountWordOccurences(atcoCmds, wordSeqCntMap);
  CountCmdOccurences(atcoCmds, cmdSeqCntMap, allowedCmdSet);

  OrderCountedWords(wordSeqCntMap, wordCntOrderedSet);
  OrderCountedCmds(cmdSeqCntMap, cmdCntOrderedSet);

  std::cout << "Numbers from utterances of file: '" + inFilePath + "':"
            << "\n";
  for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i) {
    AtcoCommand atcoCommand = atcoCmds.Get(i);
    NumberExtractor numEx{atcoCommand.GetWordSequence()};
    numEx.PerformFullExtraction();
    std::cout << "'" + atcoCommand.GetWordSequence() + "' results in:"
              << "\n";
    for (int j = 0; j < numEx.GetExtractedNumbersCnt(); ++j) {
      if (numEx.IsNumberDouble(j))
        std::cout << "  " << numEx.GetNumberAsDouble(j);
      else
        std::cout << "  " << numEx.GetNumberAsInt(j);
    }
    std::cout << "\n";
  }

  std::cout << "ActoCommands:"
            << "\n";
  for (int i = 0; i < atcoCmds.GetCountedUtterances(); ++i) {
    AtcoCommand atcoCommand = atcoCmds.Get(i);
    FileName fileName = atcoCommand.GetFileName();
    std::cout << atcoCommand.GetFileName().GetName() << " ";
    std::cout << "(" << fileName.GetDay() << "-" << fileName.GetMonth() << "-"
              << fileName.GetYear() << ",";
    std::cout << " " << fileName.GetHour() << ":" << fileName.GetMinute() << ":"
              << fileName.GetSeconds() << ":" << fileName.GetMilliseconds()
              << "):\n";
    std::cout << "  " << atcoCommand.GetWordSequence() << "\n";
    for (int j = 0; j < atcoCommand.GetCmdCnt(); j++) {
      std::cout << "   " << atcoCommand.GetCommand(j) << ";  ";
    }
    std::cout << "\n";
  }

  std::cout << "Top 10 Words:"
            << "\n";
  int i = 0;
  for (const auto &iter : wordCntOrderedSet) {
    std::cout << i << ".   " << iter.first << ":" << iter.second << "\n";
    if (i == 9 || iter.first.empty()) {
      break;
    }
    i++;
  }
  std::cout << "Top 1 Command(s):"
            << "\n";
  int highestCount = 0;
  for (const auto &iter : cmdCntOrderedSet) {
    if (iter.second <
        highestCount) { // as soon as an element in the set has a
                        // lower count than the previous one break
                        // - this way only the top commands/command
                        // (multiple if the yhave the same count)
      break;
    }
    highestCount = iter.second;
    std::cout << iter.first << ":" << iter.second << "\n";
  }
  std::cout << "Top 10 Commands:"
            << "\n";
  i = 0;
  for (const auto &iter : cmdCntOrderedSet) {
    std::cout << i << ".   " << iter.first << ":" << iter.second << "\n";
    if (i == 9 || iter.first.empty()) {
      break;
    }
    i++;
  }
}