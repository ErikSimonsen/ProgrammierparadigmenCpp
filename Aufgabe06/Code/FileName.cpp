#include <stdexcept>
#include "FileName.h"

FileName::FileName(const std::string &filename)
{
    this->SetName(filename);
}

void FileName::SplitName()
{
    if (this->name.length() != 23)
        throw std::invalid_argument("File Name has to be the following format: YYYY-MM-DD__hh-mm-ss-ms (23 characters long)");
    try
    {
        this->year = std::stoi(this->name.substr(0, 4));
        this->month = std::stoi(this->name.substr(5, 2));
        this->day = std::stoi(this->name.substr(8, 2));
        this->hour = std::stoi(this->name.substr(12, 2));
        this->minute = std::stoi(this->name.substr(15, 2));
        this->seconds = std::stoi(this->name.substr(18, 2));
        this->milliseconds = std::stoi(this->name.substr(21, 2));
    }
    catch (std::invalid_argument&)
    {
        throw std::invalid_argument("File Name has to be the following format: YYYY-MM-DD__hh-mm-ss-ms. ");
    }
}

void FileName::SetName(const std::string &val)
{
    this->name = val;
    this->SplitName();
}
// Getters
std::string FileName::GetName() const
{
    return this->name;
}
int FileName::GetYear() const
{
    return this->year;
}
int FileName::GetMonth() const
{
    return this->month;
}
int FileName::GetDay() const
{
    return this->day;
}
int FileName::GetHour() const
{
    return this->hour;
}
int FileName::GetMinute() const
{
    return this->minute;
}
int FileName::GetSeconds() const
{
    return this->seconds;
}
int FileName::GetMilliseconds() const
{
    return this->milliseconds;
}