#ifndef AUFGABE8_NATOALPHABET_H
#define AUFGABE8_NATOALPHABET_H

#include <string>
#include <unordered_map>
struct NATOAlphabet {
public:
  static bool IsInNatoAlphabet(const std::string searchString);
  static std::string GetNatoRepresentation(const std::string searchString);
  static bool IsMultiplier(const std::string searchString);

private:
  // declare as static so they can also get initialized outside of the class
  // definition (otherwise they would need to be initialized in the constructor)
  static const std::unordered_map<std::string, int> natoToMultipliers;
  static const std::unordered_map<std::string, char> natoToNumbers;
  static const std::unordered_map<std::string, int> numbersToNatoMultipleDigits;
  static const std::unordered_map<std::string, char> natoToLetters;
  static const std::unordered_map<std::string, std::string> designators;
  static bool IsNumber(std::string searchString);
  static bool IsMultipleDigit(std::string searchString);
  static bool IsLetter(std::string searchString);
  static bool IsDesignator(std::string searchString);
  // forbid to initialize an instance of this struct -> static struct
  NATOAlphabet() {}
};

#endif // AUFGABE8_NATOALPHABET_H