#include <stdexcept>
#include "FileName.h"

FileName::FileName(const std::string &filename)
{
    SetName(filename);
}

void FileName::SplitName()
{
    if (name.length() != 23)
        throw std::invalid_argument("File Name has to be the following format: YYYY-MM-DD__hh-mm-ss-ms (23 characters long)");
    try
    {
        year = std::stoi(name.substr(0, 4));
        month = std::stoi(name.substr(5, 2));
        day = std::stoi(name.substr(8, 2));
        hour = std::stoi(name.substr(12, 2));
        minute = std::stoi(name.substr(15, 2));
        seconds = std::stoi(name.substr(18, 2));
        milliseconds = std::stoi(name.substr(21, 2));
    }
    catch (std::invalid_argument&)
    {
        throw std::invalid_argument("File Name has to be the following format: YYYY-MM-DD__hh-mm-ss-ms. ");
    }
}

void FileName::SetName(const std::string &val)
{
    name = val;
    SplitName();
}
// Getters
std::string FileName::GetName() const
{
    return name;
}
int FileName::GetYear() const
{
    return year;
}
int FileName::GetMonth() const
{
    return month;
}
int FileName::GetDay() const
{
    return day;
}
int FileName::GetHour() const
{
    return hour;
}
int FileName::GetMinute() const
{
    return minute;
}
int FileName::GetSeconds() const
{
    return seconds;
}
int FileName::GetMilliseconds() const
{
    return milliseconds;
}