#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <map>
#include <set>

typedef std::map<std::string, int, std::less<>> WordSeqCntMap;
typedef std::map<std::string, int, std::less<>> CmdSeqCntMap;
typedef std::set<std::string> AllowedCmdSet;

void fillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile);

bool isLineWordSequence(std::string &line);

bool isLineCommandSequence(std::string &line);

bool isLineTimestamp(std::string &line);

std::string trim(const std::string &string);

int main() {
    WordSeqCntMap wordSeqCntMap;
    AllowedCmdSet allowedCmdSet;
    CmdSeqCntMap cmdSeqCntMap;
    std::ifstream infile("WordSeqPlusCmds2.txt");
    std::ifstream inTypes("expectedTypes.txt");
    if (!infile) {
        std::cerr << "Die angegebene Datei für die Word-Sequenzen konnte nicht gefunden werden.";
        return -1;
    }
    if (!inTypes) {
        std::cerr << "Die angegebene Datei für die erlaubten Kommandos konnte nicht gefunden werden.";
        return -1;
    }
    fillAllowedCommands(allowedCmdSet, inTypes);
    std::string line;
    std::string word;
    while (getline(infile, line)) {
        //timestamp line
        if (isLineTimestamp(line)) {
            continue;
        }
        std::stringstream strStream(line);
        //word sequence line
        if (isLineWordSequence(line)) {
            while (strStream >> word) {
                wordSeqCntMap[word]++;
            }
        }
        //command sequence line
        if (isLineCommandSequence(line)) {
            //only the first 3 words in the command sequence line are relevant for us. The third word (typeExtended) can belong to the type (but does not have to))
            std::string callSign, type, typeExtended;

            strStream >> callSign >> type >> typeExtended;

            //check if the type consists of two words and exists in the set of allowed commands. If so increment it and make the next loop iteration
            //otherwise the type (this time only with a single word) will be checked for existence again
            if (!typeExtended.empty() && !isdigit(typeExtended[0])) {
                std::string fullType = type.append(" " + typeExtended);
                if (allowedCmdSet.find(fullType) != allowedCmdSet.end()) {
                    cmdSeqCntMap[fullType]++;
                    continue;
                }
            }

            if (allowedCmdSet.find(type) != allowedCmdSet.end()) {
                cmdSeqCntMap[type]++;
            }
        }
    }
    std::cout << "WordSequence Count:" << "\n";
    for (auto &iter: wordSeqCntMap) {
        std::cout << iter.first << " : " << iter.second << "\n";
    }
    std::cout << "AllowedCommands" << "\n";
    for (auto &iter: allowedCmdSet) {
        std::cout << iter << "\n";
    }
    std::cout << "CommandSequence Count:" << "\n";
    for (auto &iter: cmdSeqCntMap) {
        std::cout << iter.first << " : " << iter.second << "\n";
    }
}

void fillAllowedCommands(AllowedCmdSet &allowedCmdSet, std::ifstream &infile) {
    std::string line;
    std::string word;
    //read each line and then parse the line with a stringstream with the comma delimiter into words
    while (getline(infile, line)) {
        std::stringstream strStream(line);
        while (getline(strStream, word, ',')) {
            allowedCmdSet.insert(trim(word));
        }
    }
}

bool isLineWordSequence(std::string &line) {
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && islower(line[3]);
}

bool isLineCommandSequence(std::string &line) {
    return isblank(line[0]) && isblank(line[1]) && isblank(line[2]) && isupper(line[3]);
}

bool isLineTimestamp(std::string &line) {
    return isdigit(line[0]) && line[0] == '2';
}

/**
 *
 * @param a reference to the string to trim
 * @return if the string had trailing or leading whitespaces the substring of its first character until its last character is returned
 * otherwise the original string is returned
 */
std::string trim(const std::string &string) {
    std::size_t start = string.find_first_not_of(' ');
    std::size_t end = string.find_last_not_of(' ');
    return start == end ? string : string.substr(start, end + 1);
}
