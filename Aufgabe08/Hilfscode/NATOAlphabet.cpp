// Copyright (C) 2019, …
// Deutsches Zentrum fuer Luft- und Raumfahrt e.V.
// German Aerospace Center
//
// Institut fuer Flugfuehrung/Institute of Flight Guidance
// Tel. +49 531 295 2500, Fax: +49 531 295 2501
// WWW: http://www.bs.dlr.de/FL/
//
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information of the German Aerospace Center
// and are protected by the copyright law. They may not be disclosed to
// third parties or copied or duplicated in any form,in whole or in part,
// without the prior written consent of the German Aerospace Center

/*!
\file NATOAlphabet.cpp

\author Sven Püschel (SP)

\date 2019-10-02 SP : first version
\date 2020-04-11 HHe: IsItNatoDigitCombinedWord is new
\date 2020-12-13 HHe: deleting some methods to handover to ostfalia students

*/

// add necessary includes and header file which access these unordered_maps
#include "NATOAlphabet.h"
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>

bool NATOAlphabet::IsInNatoAlphabet(const std::string searchString) {
  if (searchString.empty())
    return false;
  if (IsNumber(searchString) || IsMultipleDigit(searchString) ||
      IsLetter(searchString) || IsDesignator(searchString) ||
      IsMultiplier(searchString)) {
    return true;
  } else {
    return false;
  }
}

std::string
NATOAlphabet::GetNatoRepresentation(const std::string searchString) {
  // search in every map, or make previous check in which map the element is
  std::string natoRepresentation;
  if (IsNumber(searchString)) {
    auto iter = natoToNumbers.find(searchString);
    natoRepresentation = iter->second;
  } else if (IsMultipleDigit(searchString)) {
    auto iter = numbersToNatoMultipleDigits.find(searchString);
    natoRepresentation = std::to_string(iter->second);
  } else if (IsLetter(searchString)) {
    auto iter = natoToLetters.find(searchString);
    natoRepresentation = iter->second;
  } else if (IsDesignator(searchString)) {
    auto iter = designators.find(searchString);
    natoRepresentation = iter->second;
  } else if (IsMultiplier(searchString)) {
    auto iter = natoToMultipliers.find(searchString);
    natoRepresentation = std::to_string(iter->second);
  }

  return natoRepresentation;
}

bool NATOAlphabet::IsNumber(const std::string searchString) {
  auto iter = natoToNumbers.find(searchString);
  if (iter == natoToNumbers.end())
    return false;
  return true;
}
bool NATOAlphabet::IsMultipleDigit(const std::string searchString) {
  auto iter = numbersToNatoMultipleDigits.find(searchString);
  if (iter == numbersToNatoMultipleDigits.end())
    return false;
  return true;
}
bool NATOAlphabet::IsLetter(const std::string searchString) {
  auto iter = natoToLetters.find(searchString);
  if (iter == natoToLetters.end())
    return false;
  return true;
}
bool NATOAlphabet::IsDesignator(const std::string searchString) {
  auto iter = designators.find(searchString);
  if (iter == designators.end())
    return false;
  return true;
}
bool NATOAlphabet::IsMultiplier(const std::string searchString) {
  auto iter = natoToMultipliers.find(searchString);
  if (iter == natoToMultipliers.end())
    return false;
  return true;
}
const std::unordered_map<std::string, int> NATOAlphabet::natoToMultipliers = {
    {"double", 2}, {"triple", 3}};
const std::unordered_map<std::string, char> NATOAlphabet::natoToNumbers = {
    {"one", '1'},  {"two", '2'},  {"three", '3'},  {"four", '4'},
    {"five", '5'}, {"six", '6'},  {"seven", '7'},  {"eight", '8'},
    {"nine", '9'}, {"zero", '0'}, {"decimal", '.'}};

const std::unordered_map<std::string, int>
    NATOAlphabet::numbersToNatoMultipleDigits = {
        {"ten", 10},      {"eleven", 11},    {"twelve", 12},
        {"thirteen", 13}, {"fourteen", 14},  {"fifteen", 15},
        {"sixteen", 16},  {"seventeen", 17}, {"eighteen", 18},
        {"nineteen", 19}, {"twenty", 20},    {"thirty", 30},
        {"fourty", 40},   {"fifty", 50},     {"sixty", 60},
        {"seventy", 70},  {"eighty", 80},    {"ninety", 90},
        {"hundred", 100}, {"thousand", 1000}};

const std::unordered_map<std::string, char> NATOAlphabet::natoToLetters = {
    {"alfa", 'A'},    {"bravo", 'B'},    {"charlie", 'C'},
    {"delta", 'D'}, // delta is also an airline name
    {"echo", 'E'},    {"fox", 'F'},      {"foxtrot", 'F'}, // also fox
    {"golf", 'G'},    {"hotel", 'H'},    {"india", 'I'},
    {"juliett", 'J'}, {"kilo", 'K'},     {"lima", 'L'},
    {"mike", 'M'},    {"november", 'N'}, {"oscar", 'O'},
    {"papa", 'P'},    {"quebec", 'Q'},   {"romeo", 'R'},
    {"sierra", 'S'},

    {"tango", 'T'},   {"uniform", 'U'},  {"victor", 'V'},
    {"whiskey", 'W'}, {"x-ray", 'X'},    {"yankee", 'Y'},
    {"zulu", 'Z'}};
const std::unordered_map<std::string, std::string> NATOAlphabet::designators = {
    {"b_air", "ABP"},
    {"canada", "ACA"},
    {"airest", "AEG"},
    {"eastern", "AEG"},
    {"east air", "AEG"},
    {"east", "AEG"},
    {"air_france", "AFR"},
    {"france", "AFR"},
    {"lupus", "AYY"},
    {"air_berlin", "BER"},
    {"berlin", "BER"},
    {"speed bird", "BAW"},
    {"speed_bird", "BAW"},
    {"speedbird", "BAW"},
    {"air_china", "CCA"},
    {"china", "CCA"},
    {"csa", "CSA"},
    {"czech", "CSA"},
    {"CSA", "CSA"},
    {"easy_jet", "EZY"},
    {"easy jet", "EZY"},
    {"united states of america", "GTW"},
    {"lauda_motion", "LDM"},
    {"lauda", "LDM"},
    {"motion", "LDM"},
    {"lot", "LOT"},
    {"swiss air", "SWR"},
    {"swiss_air", "SWR"},
    {"travel air", "TVL"},
    {"travel service", "TVL"},
    {"sky_travel", "TVS"},
    {"sky travel", "TVS"},
    {"wizzair", "WZZ"},
    {"wizz_air", "WZZ"},
    {"wizz air", "WZZ"},
    {"lufthansa", "DLH"},
    {"hansa", "DLH"},
    {"lufthansa cargo", "GEC"},
    {"lufthansa india", "LCI"},
    {"lufthansa technik", "LHT"},
    {"delta", "DAL"},
    {"deltair", "DAT"},
    {"delta air", "DAT"},
    {"capital delta", "CPD"},
    {"klm heli", "KLM"},
    {"klm", "KLM"},
    {"KLM", "KLM"},
    {"tarom", "ROT"},
    {"enter", "ENT"},
    {"enter air", "ENT"},
    {"ryan air", "RYR"},
    {"ryan_air", "RYR"},
    {"ryanair", "RYR"},
    {"austrian", "AUA"},
    {"austrian airlines", "AUA"},
    {"snowcap", "MHV"},
    {"snow cap", "MHV"},
    {"snowcab", "MHV"},
    {"snow cab", "MHV"},
    {"flyniki", "NLY"},
    {"fly niki", "NLY"},
    {"fly_niki", "NLY"},
    {"scandinavian", "SAS"}};