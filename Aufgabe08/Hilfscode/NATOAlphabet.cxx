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

   static const std::unordered_map<char, std::string> numbersToNato =
   {
      { '1', "one" },
      { '2', "two" },
      { '3', "three" },
      { '4', "four" },
      { '5', "five" },
      { '6', "six" },
      { '7', "seven" },
      { '8', "eight" },
      { '9', "nine" },
      { '0', "zero" },
      { '.', "decimal" }
   };

   static const std::unordered_map<std::string, int> numbersToNatoMultipleDigits =
   {
      { "ten", 10 },
   { "eleven",  11 },
   { "twelve",  12 },
   { "thirteen",  13 },
   { "fourteen",  14 },
   { "fifteen",  15 },
   { "sixteen",  16 },
   { "seventeen",  17 },
   { "eighteen",  18 },
   { "nineteen",  19 },

   { "twenty",  20 },
   { "thirty",  30 },
   { "fourty",  40 },
   { "fifty",  50 },
   { "sixty",  60 },
   { "seventy",  70 },
   { "eighty",  80 },
   { "ninety",  90 },

   { "hundred",  100 },
   { "thousand",  1000 }
   };



   static const std::unordered_map<char, std::string> lettersToNato =
   {
      { 'A', "alfa" },
      { 'B', "bravo" },
      { 'C', "charlie" },
      { 'D', "delta" }, // delta is also an airline name
      { 'E', "echo" },
      { 'F', "foxtrot" }, // also fox
      { 'G', "golf" },
      { 'H', "hotel" },
      { 'I', "india" },
      { 'J', "juliett" },
      { 'K', "kilo" },
      { 'L', "lima" },
      { 'M', "mike" },
      { 'N', "november" },
      { 'O', "oscar" },
      { 'P', "papa" },
      { 'Q', "quebec" },
      { 'R', "romeo" },
      { 'S', "sierra" },
      { 'T', "tango" },
      { 'U', "uniform" },
      { 'V', "victor" },
      { 'W', "whiskey" },
      { 'X', "x-ray" },
      { 'Y', "yankee" },
      { 'Z', "zulu" }
   };

