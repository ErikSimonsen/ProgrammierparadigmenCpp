#include "Processing.h"
#include <iostream>
#include <vector>

void SpecialCallsignCheckFilesHHe() {
  std::string filePath = "testAssets/CallsignExtractionBachelor.txt";
  std::vector<std::string> expectedValues = {
      "OKTNT",   "CSA908",  "KLM1352", "CSA908",  "ROT226A",
      "ROT226A", "TVS1263", "ENT852F", "ENT852P", "RYR9768D21V"};
  std::cout << "CallsignExtractionBachelor.txt\n";
  ReadUtteranceCheckCallsign(filePath, true, expectedValues);

  expectedValues = {"OKTNT",   "CSA908",  "KLM1352", "CSA908",  "ROT226A",
                    "ROT226A", "TVS1263", "ENT852F", "DAL87DV", "GTW2F1F"};
  filePath = "testAssets/CallsignExtractionMaster.txt";
  std::cout << "\nCallsignExtractionMaster.txt\n";
  ReadUtteranceCheckCallsign(filePath, true, expectedValues);

  expectedValues = {"GTWAB1F", "GTWAB1",  "TVS1263", "TVS1263", "TVS81D3",
                    "TVS1126", "TVS1126", "TVS112",  "TVS1F2A", "TVS112A",
                    "AUA666E", "AUA200E", "AUA7000", "TVS1126"};
  filePath = "testAssets/BigChallenges.txt";
  std::cout << "\nBigChallenges.txt\n";
  ReadUtteranceCheckCallsign(filePath, true, expectedValues);
}

void SpecialNumberCheckFilesHHe() {
  const std::vector<std::vector<double>> expectedNumbers1 = {
      {118.850}, // one one eight decimal eight five zero
      {120},     // climb flight level one two zero
      {120.075}, // contact praha radar one two zero decimal zero seven five
      {350},     // CSA nine zero eight climb flight level three five zero
      {},        // tarom two two six alfa praha radar report present altitude
      {4000},    // tarom two two six alfa radar radar contact correct climb
                 // altitude four thousand feet
      {532},     // ... proceed direct papa romeo five three two
      {20},      // ...  fox turn right heading zero two zero
      {200,
       24}, // ...right heading two hundred cleared ils approach runway two four
      {2500} // ryan air nine seven six eight delta two one victor descend
             // altitude two thousand five hundred feet
  };

  std::string filePath = "testAssets/CallsignExtractionBachelor.txt";
  std::cout << "NumberExtractionBachelor.txt\n";
  ReadUtteranceExtractNumbers(filePath, expectedNumbers1, true);

  const std::vector<std::vector<double>> expectedNumbers2 = {
      {118.650}, // praha radar contact karlovy vary radar one one eight decimal
                 // six five zero naslysenou
      {120},     // climb flight level one twenty
      {120.075}, // contact praha radar one two zero decimal zero seventy five
                 // naslysenou bye bye
      {150},     // climb flight level one hundred fifty
      {},        // praha radar report present altitude
      {4621},    // radar radar contact correct climb altitude four thousand six
                 // hundred twenty one
      {532},     // ... proceed direct papa romeo five three two
      {5},       // ...  turn right heading zero zero five
      {12500},   // ...descend altitude one two thousand five hundred feet
      {230} // hi united states of america two fox one foxtrot identified climb
            // flight level two three zero
  };

  filePath = "testAssets/CallsignExtractionMaster.txt";

  std::cout << "\nNumberExtractionMaster.txt\n";
  ReadUtteranceExtractNumbers(filePath, expectedNumbers2, true);

  const std::vector<std::vector<double>> expectedNumbers3 = {
      {230}, // hi united states of america alfa bravo one fox identified climb
             // flight level two three zero
      {230}, // climb flight level two three zero
      {532}, // proceed direct papa romeo five four correction papa romeo five
             // three two
      {532}, // proceed direct papa romeo five three two
      {533}, // direct to  papa romeo five double three
      {300}, // proceed direct papa romeo three hundred
      {532, 65}, // proceed direct papa romeo five three two sky_travel one one
                 // two six turn left heading zero six five
      {1032}, // sky_travel one one two alfa is correct qnh one zero three two
      {1033}, // qnh one zero three three sky_travel one foxtrot two alfa
      {},     // direct rapet sky_travel one one two alfa
      {2170}, // hello austria triple six echo direct rapet squawk two one seven
              // zero
      {},     // hello austria two hundred echo direct rapet
      {}      // hello austria seven thousand  direct rapet
      ,
      {532, 20} //  proceed direct papa romeo five three two turn left heading
                //  zero six correction turn
                //   by two zero to the left for sky_travel one one two six
  };
  filePath = "testAssets/BigChallenges.txt";

  std::cout << "\nNumberBigChallenges.txt\n";
  ReadUtteranceExtractNumbers(filePath, expectedNumbers3, true);
}
