#ifndef AUFGABE8_DATETIME_H
#define AUFGABE8_DATETIME_H

#include <string>

class FileName {
public:
  FileName() = default;
  explicit FileName(
      const std::string &
          fileName); // https://stackoverflow.com/questions/121162/what-does-the-explicit-keyword-mean

  void SetName(const std::string &fileName);
  std::string GetName() const;

  int GetYear() const;
  int GetMonth() const;
  int GetDay() const;
  int GetHour() const;
  int GetMinute() const;
  int GetSeconds() const;
  int GetMilliseconds() const;

private:
  std::string name;
  int year, month, day, hour, minute, seconds, milliseconds;
  void SplitName();
};

#endif // AUFGABE8_DATETIME_H
