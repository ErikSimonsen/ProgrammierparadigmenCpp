#include "Util.h"
/**
 * Gets the position of the first and last character that are not equal to the specified characters to be trimmed (whitespace etc)
 * and then returns the substring beginning from the position of the first character to the position of the last character found.
 * @param a reference to the string to trim
 * @return if the string had trailing or leading whitespaces the substring of its first character until its last character is returned
 * otherwise the original string is returned
 */
std::string Trim(const std::string &str) {
    std::size_t start = str.find_first_not_of(' ');
    //indicates that no character was found that is different to the specified character (whitespace)
    // -> which means that the string either was empty or consists only of whitespaces
    if(start == std::string::npos){
        return std::string();
    }
    std::size_t end = str.find_last_not_of(' ');
    return str.substr(start, (end-start) + 1);
}
